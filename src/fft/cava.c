/*
* to keep coherent styling autostyle with:
* astyle --style=linux --pad-oper --pad-header --max-code-length=80 --keep-one-line-blocks --keep-one-line-statements --convert-tabs --indent=tab cava.c
*/

#include <alloca.h>
#include <locale.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <alsa/asoundlib.h>
#include <sys/ioctl.h>
#include <fftw3.h>
#define PI 3.14159265358979323846
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <pthread.h>

#ifdef __GNUC__
// curses.h or other sources may already define
#undef  GCC_UNUSED
#define GCC_UNUSED __attribute__((unused))
#else
#define GCC_UNUSED /* nothing */
#endif

bool DEBUG=true;

  struct sigaction old_action;
  int M = 2048;
  int shared[2048];
  int format = -1;
  unsigned int rate = 0;


//ALSA audio listner
void* music(void* data)
{
	signed char *buffer;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int val;
	snd_pcm_uframes_t frames;
	char *device = ((char*)data);
	val = 44100;
	int i, n, o, size, dir, err, lo;
	int tempr, templ;
	int radj, ladj;

        //**init sound device***//

	if ((err = snd_pcm_open(&handle, device,  SND_PCM_STREAM_CAPTURE , 0) < 0))
		printf("error opening stream:    %s\n", snd_strerror(err) );

	snd_pcm_hw_params_alloca(&params);//assembling params
	snd_pcm_hw_params_any (handle, params);//setting defaults or something
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);//interleeaved mode right left right left
	snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE); //trying to set 16bit
	snd_pcm_hw_params_set_channels(handle, params, 2);//asuming stereo
	val = 44100;
	snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);//trying 44100 rate
	frames = 256;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir); //number of frames pr read

	err = snd_pcm_hw_params(handle, params); //atempting to set params
	if (err < 0) {
		fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(err));
		exit(1);
	}

	snd_pcm_hw_params_get_format(params, (snd_pcm_format_t * )&val); //getting actual format
//convverting result to number of bits
	if (val < 6)format = 16;
	else if (val > 5 && val < 10)format = 24;
	else if (val > 9)format = 32;

	snd_pcm_hw_params_get_rate( params, &rate, &dir); //getting rate

	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	snd_pcm_hw_params_get_period_time(params,  &val, &dir);

	size = frames * (format / 8) * 2; // frames * bits/8 * 2 channels
	buffer = malloc(size);
	radj = format / 4; //adjustments for interleaved
	ladj = format / 8;
	o = 0;
	while (1) {

		err = snd_pcm_readi(handle, buffer, frames);
		if (err == -EPIPE) {
			/* EPIPE means overrun */
			fprintf(stderr, "overrun occurred\n");
			snd_pcm_prepare(handle);
		} else if (err < 0) {
			fprintf(stderr, "error from read: %s\n", snd_strerror(err));
		} else if (err != (int)frames) {
			fprintf(stderr, "short read, read %d %d frames\n", err, (int)frames);
		}

		//sorting out one channel and only biggest octet
		n = 0; //frame counter
		for (i = 0; i < size ; i = i + (ladj) * 2) {
			//              left                            right
			//structuere [litte],[litte],[big],[big],[litte],[litte],[big],[big]...

			//first channel
			tempr = ((buffer[i + (radj) - 1 ] <<
			          2)); //using the 10 upper bits this whould give me a vert res of 1024, enough...
			lo = ((buffer[i + (radj) - 2] >> 6));
			if (lo < 0)lo = lo + 4;
			if (tempr >= 0)tempr = tempr + lo;
			if (tempr < 0)tempr = tempr - lo;

			//other channel
			templ = (buffer[i + (ladj) - 1] << 2);
			lo = (buffer[i + (ladj) - 2] >> 6);
			if (lo < 0)lo = lo + 4;
			if (templ >= 0)templ = templ + lo;
			else templ = templ - lo;

			//adding channels and storing it int the buffer
			shared[o] = (tempr + templ) / 2;
			o++;
			if (o == M - 1)o = 0;

			//shifing ringbuffer one to the left, this ended up using to much cpu..
			//for(o=0;o<M-1;o++) shared[o]=shared[o+1];
			n++;
		}
	}
}

//FIFO audio listner
void* fifomusic(void* data)
{
	int fd;
	int n = 0;
	signed char buf[1024];
	int tempr, templ, lo;
	int q, i;
	int t = 0;
	int size = 1024;
	char *path = ((char*)data);
	int bytes = 0;
	int flags;
	struct timespec req = { .tv_sec = 0, .tv_nsec = 0 };

	req.tv_sec = 0;
	req.tv_nsec = 10000000;


	fd = open(path, O_RDONLY);
	flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	while (1) {

		bytes = read(fd, buf, sizeof(buf));

		if (bytes == -1) { //if no bytes read sleep 10ms and zero
			nanosleep (&req, NULL);
			t++;
			if (t > 10) {
				for (i = 0; i < M; i++)shared[i] = 0;
				t = 0;
			}
		} else { //if bytes read go ahead
			t = 0;
			for (q = 0; q < (size / 4); q++) {
				tempr = ( buf[size - 4 * q - 1] << 2);
				lo =  ( buf[size - 4 * q ] >> 6);
				if (lo < 0)lo = lo + 4;
				if (tempr >= 0)tempr = tempr + lo;
				if (tempr < 0)tempr = tempr - lo;

				templ = ( buf[size - 4 * q - 3] << 2);
				lo =  ( buf[size - 4 * q - 2] >> 6);

				if (lo < 0)lo = lo + 4;
				if (templ >= 0)templ = templ + lo;
				else templ = templ - lo;

				shared[n] = (tempr + templ) / 2;

				n++;
				if (n == M - 1)n = 0;
			}
		}
	}
	close(fd);
}

int main(int argc, char **argv)
{
	pthread_t  p_thread;
	int        thr_id GCC_UNUSED;
	char *input = "alsa";
	int im = 1;
	//char *device = "hw:1,1";
	char *device = "default";
	char *path = "/tmp/mpd.fifo";
	float fc[200];//={150.223,297.972,689.062,1470,3150,5512.5,11025,18000};
	float fr[200];//={0.00340905,0.0067567,0.015625,0.0333,0.07142857,0.125,0.25,0.4};
	int lcf[200], hcf[200];
	float f[200];
	float fmem[200];
	int flast[200];
	float peak[201];
	int y[M / 2 + 1];
	long int lpeak, hpeak;
	int bands = 5;
	int sleep = 0;
	int i, n, o, bw, width, height, c, rest, virt, fixedbands;
	int autoband = 1;
//long int peakhist[bands][400];
	float temp;
	struct winsize w;
	double in[2 * (M / 2 + 1)];
	fftw_complex out[M / 2 + 1][2];
	fftw_plan p;
	char *color;
	int col = 36;
	int bgcol = 0;
	int sens = 100;
#ifndef DEBUG
	int move = 0;
#endif
	int fall[200];
	float fpeak[200];
	float k[200];
	float g;
	int framerate = 60;
	float smooth[64] = {5, 4.5, 4, 3, 2, 1.5, 1.25, 1.5, 1.5, 1.25, 1.25, 1.5, 1.25, 1.25, 1.5, 2, 2, 1.75, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.75, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
	float sm = 1.25; //min val from the above array
	struct timespec req = { .tv_sec = 0, .tv_nsec = 0 };
	char *usage = "";
//**END INIT**//

	setlocale(LC_ALL, "");

	for (i = 0; i < 200; i++) {
		flast[i] = 0;
		fall[i] = 0;
		fpeak[i] = 0;
		fmem[i] = 0;
	}
	for (i = 0; i < M; i++)shared[i] = 0;

//**arg handler**//

//**ctrl c handler**//
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	//action.sa_handler = &sigint_handler;
	sigaction(SIGINT, &action, &old_action);



	n = 0;
	if (im == 1) {
//**watintg for audio to be ready**//
		thr_id = pthread_create(&p_thread, NULL, music, (void*)device); //starting alsamusic listner
		while (format == -1 || rate == 0) {
			req.tv_sec = 0;
			req.tv_nsec = 1000000;
			nanosleep (&req, NULL);
			n++;
			if (n > 2000) {
#ifdef DEBUG
				printf("could not get rate and or format, problems with audoi thread? quiting...\n");
#endif
				exit(1);
			}
		}
#ifdef DEBUG
		printf("got format: %d and rate %d\n", format, rate);
#endif

	}

	if (im == 2) {
		thr_id = pthread_create(&p_thread, NULL, fifomusic,
		                        (void*)path); //starting fifomusic listner
		rate = 44100;
		format = 16;
	}

	p =  fftw_plan_dft_r2c_1d(M, in, *out, FFTW_MEASURE); //planning to rock


//**getting h*w of term**//
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	while  (1) {//jumbing back to this loop means that you resized the screen
		//getting orignial numbers of bands incase of resize
		if (autoband == 1)  {
			bands = 25;
		} else bands = fixedbands;


		if (bands > (int)w.ws_col / 2 - 1)bands = (int)w.ws_col / 2 -
			                1; //handle for user setting to many bars
		height = (int)w.ws_row - 1;
		width = (int)w.ws_col - bands - 1;
#ifndef DEBUG
		int matrix[width][height];
		for (i = 0; i < width; i++) {
			for (n = 0; n < height; n++) {
				matrix[i][n] = 0;
			}
		}
#endif
		bw = width / bands;

		g = ((float)height / 1000) * pow((60 / (float)framerate),
		                                 2.5); //calculating gravity

//if no bands are selected it tries to padd the default 20 if there is extra room
		if (autoband == 1) bands = bands + (((w.ws_col) - (bw * bands + bands - 1)) /
			                                    (bw + 1));
		width = (int)w.ws_col - bands - 1;
//checks if there is stil extra room, will use this to center
		rest = (((w.ws_col) - (bw * bands + bands - 1)));
		if (rest < 0)rest = 0;

#ifdef DEBUG
		printf("hoyde: %d bredde: %d bands:%d bandbredde: %d rest: %d\n",
		       (int)w.ws_row,
		       (int)w.ws_col, bands, bw, rest);
#endif

//**calculating cutof frequencies**/
		for (n = 0; n < bands + 1; n++) {
			fc[n] = 10000 * pow(10, -2.37 + ((((float)n + 1) / ((float)bands + 1)) *
			                                 2.37)); //decided to cut it at 10k, little interesting to hear above
			fr[n] = fc[n] / (rate /
			                 2); //remember nyquist!, pr my calculations this should be rate/2 and  nyquist freq in M/2 but testing shows it is not... or maybe the nq freq is in M/4
			lcf[n] = fr[n] * (M /
			                  4); //lfc stores the lower cut frequency fro each band in the fft out buffer

			if (n != 0) {
				hcf[n - 1] = lcf[n] - 1;
				if (lcf[n] <= lcf[n - 1])lcf[n] = lcf[n - 1] +
					                                  1; //pushing the spectrum up if the expe function gets "clumped"
				hcf[n - 1] = lcf[n] - 1;
			}

#ifdef DEBUG
			if (n != 0) {
				printf("%d: %f -> %f (%d -> %d) \n", n, fc[n - 1], fc[n], lcf[n - 1],
				       hcf[n - 1]);
			}
#endif
		}
//exit(1);

//constants to weigh signal to frequency
		for (n = 0; n < bands;
		     n++)k[n] = ((float)height * pow(log(lcf[n] + 1),
			                                     2 + ((float)bands / 75))) / (1024 * (M /
			                                                     16)); // the log(lcf[n]) is because higher frequencys are usally lower ine effect in music



//**preparing screen**//
		virt = system("setfont cava.psf  >/dev/null 2>&1");
#ifndef DEBUG
		system("setterm -cursor off");
		system("setterm -blank 0");
//resetting console
		printf("\033[0m\n");
		system("clear");
		system("stty -echo");

		printf("\033[%dm", col); //setting color

		printf("\033[1m"); //setting "bright" color mode, looks cooler... I think
		if (bgcol != 0)
			printf("\033[%dm", bgcol);
		{
			for (n = (height); n >= 0; n--) {
				for (i = 0; i < width + bands; i++) {

					printf(" ");//setting backround volor

				}
				printf("\n");//setting volor
			}
			printf("\033[%dA", height); //backup
		}
#endif

//debug=1;
//**start main loop**//
		while  (1) {



//**checkint if terminal windows has been resized**//

			if (virt != 0) {
				ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
				if ( ((int)w.ws_row - 1) != height || ((int)w.ws_col - bands - 1) != width) {
					break;
				}
			}

#ifdef DEBUG
			system("clear");
#endif

//**populating input buffer & checking if there is sound**//
			lpeak = 0;
			hpeak = 0;
			for (i = 0; i < (2 * (M / 2 + 1)); i++) {
				if (i < M) {
					in[i] = shared[i];
					if (shared[i] > hpeak) hpeak = shared[i];
					if (shared[i] < lpeak) lpeak = shared[i];
				} else in[i] = 0;
				// if(debug==1) printf("%d %f\n",i,in[i]);
			}
			peak[bands] = (hpeak + abs(lpeak));
			if (peak[bands] == 0)sleep++;
			else sleep = 0;

			//**if sound for the last 5 sec go ahead with fft**//
			if (sleep < framerate * 5) {

				fftw_execute(p);         //applying FFT to signal

				//seperating freq bands
				for (o = 0; o < bands; o++) {
					peak[o] = 0;

					//getting peaks
					for (i = lcf[o]; i <= hcf[o]; i++) {
						y[i] = pow(pow(*out[i][0], 2) + pow(*out[i][1], 2), 0.5); //getting r of compex
						peak[o] += y[i]; //adding upp band
					}
					peak[o] = peak[o] / i; //getting average
					temp = peak[o] * k[o] * ((float)sens /
					                         100); //multiplying with k and adjusting to sens settings
					if (temp > height)temp = height; //just in case

					//**falloff function**//

					if (temp < flast[o]) {
						f[o] = fpeak[o] - (g * fall[o] * fall[o]);
						fall[o]++;
					} else if (temp >= flast[o]) {
						f[o] = temp;
						fpeak[o] = f[o];
						fall[o] = 0;
					}

					//**smoothening**//

					fmem[o] += f[o];
					fmem[o] = fmem[o] * 0.55;
					f[o] = fmem[o];



					flast[o] = f[o]; //memmory for falloff func

					if (f[o] < 0.125)f[o] = 0.125;
#ifdef DEBUG
					printf("%d: f:%f->%f (%d->%d)peak:%f adjpeak: %f \n", o, fc[o], fc[o + 1],
					       lcf[o], hcf[o], peak[o], f[o]);
#endif
				}
				// if(debug==1){ printf("topp overall unfiltered:%f \n",peak[bands]); }

				//if(debug==1){ printf("topp overall alltime:%f \n",sum);}
			} else { //**if in sleep mode wait and continiue**//
#ifdef DEBUG
				printf("no sound detected for 3 sec, going to sleep mode\n");
#endif
				//for (i=0;i<200;i++)flast[i]=0; //zeroing memory   no more nesceseary after faloff on pauses
				//pthread_cancel(thr_id);// this didnt work to well, killing sound thread
				//wait 1 sec, then check sound again.
				req.tv_sec = 1;
				req.tv_nsec = 0;
				nanosleep (&req, NULL);
				continue;
			}

			/* MONSTERCAT STYLE EASING BY CW !aFrP90ZN26 */
			int z, m_y;
			float m_o = 64 / bands;
			for (z = 0; z < bands; z++) {
				f[z] = f[z] * sm / smooth[(int)floor(z * m_o)];
				if (f[z] < 0.125)f[z] = 0.125;
				for (m_y = z - 1; m_y >= 0; m_y--) {
					f[m_y] = max(f[z] / pow(2, z - m_y), f[m_y]);
				}
				for (m_y = z + 1; m_y < bands; m_y++) {
					f[m_y] = max(f[z] / pow(2, m_y - z), f[m_y]);
				}
			}

//**DRAWING**// -- put in function file maybe?
#ifndef DEBUG
			for (n = (height - 1); n >= 0; n--) {
				o = 0;
				move = rest / 2; //center adjustment
				//if(rest!=0)printf("\033[%dC",(rest/2));//center adjustment
				for (i = 0; i < width; i++) {

					//next bar? make a space
					if (i != 0 && i % bw == 0) {
						o++;
						if (o < bands)move++;
					}


					//draw color or blank or move+1
					if (o < bands) {     //watch so it doesnt draw to far
						if (f[o] - n < 0.125) { //blank
							if (matrix[i][n] != 0) { //change?
								if (move != 0)printf("\033[%dC", move);
								move = 0;
								printf(" ");
							} else move++; //no change, moving along
							matrix[i][n] = 0;
						} else if (f[o] - n > 1) { //color
							if (matrix[i][n] != 1) { //change?
								if (move != 0)printf("\033[%dC", move);
								move = 0;
								printf("\u2588");
							} else move++; //no change, moving along
							matrix[i][n] = 1;
						} else { //top color, finding fraction
							if (move != 0)printf("\033[%dC", move);
							move = 0;
							c = ((((f[o] - (float)n) - 0.125) / 0.875 * 7) + 1);
							if (0 < c && c < 8) {
								if (virt == 0)printf("%d", c);
								else printf("%lc", L'\u2580' + c);
							} else printf(" ");
							matrix[i][n] = 2;
						}
					}

				}

				printf("\n");//next line

			}

			printf("\033[%dA", height); //backup

			req.tv_sec = 0;
			req.tv_nsec = (1 / (float)framerate) * 1000000000; //sleeping for set us
			nanosleep (&req, NULL);
#endif
		}
	}
	return 0;
}
