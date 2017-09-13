#include<stdio.h>
#include<stdbool.h>
#include <time.h>
#include <math.h>
#include <alsa/asoundlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fftw3.h>
#define PI 3.14159265358979323846
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include <getopt.h>

struct sigaction old_action;



double f[200];

int mainFFT()
{
	int M=4096;
	signed char *buffer;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int val;
	snd_pcm_uframes_t frames;
	char *device = "default";
	float fc[200];//={150.223,297.972,689.062,1470,3150,5512.5,11025,18000};
	float fr[200];//={0.00340905,0.0067567,0.015625,0.0333,0.07142857,0.125,0.25,0.4};
	int lcf[200], hcf[200];

	double x[M];
	double peak[201];
	float y[M/2+1];
	long int lpeak,hpeak;
	int bands=20;
	int sleep=0;
	float h;
	int i, n, o, size, dir, err,xb,yb,bw,format,width,height,c,rest,virt;
	unsigned int rate;
	int autoband=1;
	//long int peakhist[bands][400];
	double temp;
	double sum=0;
	int16_t hi;
	int q=0;
	val=44100;
	struct winsize w;
	double in[2*(M/2+1)];
	fftw_complex out[M/2+1][2];
	fftw_plan p;
	struct timespec start, stop;
	double accum;
	char *color;
	int col = 37;
	int debug=1;
	//---------- THIS IS THE END OF INIT, MUST STOP PUTTING INIT BELOW



//**drawing frame**//
if(debug==0){
virt = system("setfont cava.psf");
system("setterm -cursor off");
}


//getting h*w of term
ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
if(bands>(int)w.ws_col/2-1)bands=(int)w.ws_col/2-1; //handle for user setting to many bars
height=(int)w.ws_row-1;
width=(int)w.ws_col-bands-1;
bool matrix[width][height];
bw=width/bands;

//if no bands are selected it tries to padd the default 20 if there is extra room
if(autoband==1) bands=bands+(((w.ws_col)-(bw*bands+bands-1))/(bw+1));

//checks if there is stil extra room, will use this to center
rest=(((w.ws_col)-(bw*bands+bands-1)));
if(rest<0)rest=0;

//resetting console
printf("\033[0m\n");
system("clear");

printf("\033[%dm",col);//setting volor




/*
if(debug==0){
for(yb=0;yb<w.ws_row;yb++)
{
for(xb=0;xb<w.ws_col;xb++)
{
    if(yb==0)
    {
    if(xb==0)printf("\u250c");//top left
    else if(xb==w.ws_col-1)printf("\u2510");//top right
    else printf("\u2500");//top
    }
    else if(yb==w.ws_row-1)
    {
    if(xb==0)printf("\u2514");//bottom left
    else if(xb==w.ws_col-1)printf("\u2518");//bottom right
    else printf("\u2500");//bottom
    }
    else
    {
    if (xb==0||xb==w.ws_col-1)printf("\u2502");//left and right
    else printf("%1c",32);
    }
}
if(yb!=w.ws_row-1)printf("\n");
}
printf("\r\033[%dC",1);
printf("%c[%dA",27,w.ws_row-2);//backup
fflush(stdout);
}
*/


//**init sound device***//

if ((err = snd_pcm_open(&handle, device,  SND_PCM_STREAM_CAPTURE , 0) < 0))
  printf("error opening stream:    %s\n",snd_strerror(err) );
else
 if(debug==1){ printf("open stream succes\n");  }
snd_pcm_hw_params_alloca(&params);
snd_pcm_hw_params_any (handle, params);
snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
snd_pcm_hw_params_set_channels(handle, params, 2);
val = 44100;
snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
frames = 32;
snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

err = snd_pcm_hw_params(handle, params);
if (err < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(err));
    exit(1);
}


snd_pcm_hw_params_get_period_size(params,&frames, &dir);
snd_pcm_hw_params_get_period_time(params,  &val, &dir);

snd_pcm_hw_params_get_format(params, (snd_pcm_format_t * )&val); //getting format

if(val<6)format=16;
else if(val>5&&val<10)format=24;
else if(val>9)format=32;

size = frames * (format/8)*2; /* bytes/sample * 2 channels */
buffer = (signed char *) malloc(size);

if(debug==1)printf("detected format: %d\n",format);

snd_pcm_hw_params_get_rate( params, &rate, &dir); //getting rate
if(debug==1)printf("detected rate: %d\n",rate);



//**calculating cutoff frequencies**/
for(n=0;n<bands+1;n++)
{
    fc[n]=8000*pow(10,-2+(((float)n/(float)bands)*2));//decided to cut it at 10k, little interesting to hear above
    fr[n]=fc[n]/(rate); //remember nyquist
    lcf[n]=fr[n]*(M/2+1);

    if(n!=0)hcf[n-1]=lcf[n];

    if(debug==1&&n!=0){printf("%d: %f -> %f (%d -> %d) \n",n,fc[n-1],fc[n],lcf[n-1],hcf[n-1]);}
}



p =  fftw_plan_dft_r2c_1d(M, in, *out, FFTW_MEASURE); //planning to rock

//**start main loop**//
while  (1)
{
/*
if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
}
*/

//**filling of the buffer**//
for(o=0;o<M/32;o++)
{
    err = snd_pcm_readi(handle, buffer, frames);
    if (err == -EPIPE) {
      /* EPIPE means overrun */
       if(debug==1){ fprintf(stderr, "overrun occurred\n");}
      snd_pcm_prepare(handle);
    } else if (err < 0) {
     if(debug==1){ fprintf(stderr, "error from read: %s\n",
              snd_strerror(err));}
    } else if (err != (int)frames) {
     if(debug==1){ fprintf(stderr, "short read, read %d %d frames\n", err,(int)frames);}
   }

       //sorting out one channel and only biggest octet
         n=0;//frame counter
        for (i=0; i<size ; i=i+(format/8)*2)
        {
                //              left                            right
                 //structuere [litte],[litte],[big],[big],[litte],[litte],[big],[big]...

                x[n+(int)frames*o] = (buffer[i+(format/4)-1]+buffer[i+(format/8)-1])/2;//avg of left and right

                    if(x[n+(int)frames*o]>hpeak) hpeak=x[o];
                    if(x[n+(int)frames*o]<lpeak) lpeak=x[o];
                n++;
        }
}


  if(debug==1){ system("clear");}


//**populating input buffer & checking if there is sound**//
     lpeak=0;
     hpeak=0;
    for (i=0;i<(2*(M/2+1));i++)
    {
       if(i<M)
        {
             in[i]=x[i];
             if(x[i]>hpeak) hpeak=x[i];
             if(x[i]<lpeak) lpeak=x[i];
        }
       else in[i]=0;
    // if(debug==1) printf("%d %f\n",i,in[i]);
    }
    peak[bands]=(hpeak+abs(lpeak));

    //**if sound go ahead with fft**//
    if (peak[bands]!=0)
    {
        sleep=0; //wake if was sleepy

        fftw_execute(p);         //applying FFT to signal

       //saving freq domian of input signal
        for (i=0;i<M/2+1;i++)
        {
        y[i]=pow(pow(*out[i][0],2)+pow(*out[i][1],2),0.5);
        }

        //seperating freq bands
        for(o=0;o<bands;o++)
        {
            peak[o]=0;

            //getting peaks
             for (i=lcf[o];i<=hcf[o];i++)
                {
                if(y[i]>peak[o]) peak[o]=y[i];
                }

            //if (peak[o]>sum)sum=peak[o]; peakest values are ussaly around 50k but i have seen 113000
            //divides on 200 because of complex mplification
            //mulitplise by log of frequency probably because of eq master cd standard, riaa...?


            f[o]=((peak[o]*(float)height*log(hcf[o]+10)*log(hcf[o]+10))) /(254*(M/8)*(log(hcf[bands-1]))) ;  //weighing signal to height and frequency

            if(f[o]>height)f[o]=height;//just in case

            if(debug==1){ printf("%d: f:%f->%f peak:%f adjpeak: %f \n",o,fc[o],fc[o+1],peak[o],f[o]);}
        }
      if(debug==1){ printf("topp overall unfiltered:%f \n",peak[bands]);

        }
       //if(debug==1){ printf("topp overall alltime:%f \n",sum);}
    }
    else//**if no signal don't bother**//
    {
        if (sleep>(rate*5)/M)//if no signal for 5 sec, go to sleep mode
        {
            if(debug==1)printf("no sound detected for 5 sec, going to sleep mode\n");
            usleep(1*1000000);//wait one sec, then check sound again. Maybe break, close and reopen?
            continue;
        }
        if(debug==1)printf("no sound detected, trying again\n");
        sleep++;
        continue;
    }



printf("\033[%dA",height);//backup



}

fftw_destroy_plan(p);
return 0;
}
    
