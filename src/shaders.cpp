	void verticalBlur() throws LWJGLException {
		//now we can render to the screen using the vertical blur shader

		//send the screen-size projection matrix to the blurShader
		batch.resize(Display.getWidth(), Display.getHeight());

		//apply the blur only along Y-axis
		blurShader.setUniformf("dir", 0f, 1f);

		//update Y-axis blur radius based on mouse
		float mouseYAmt = (Display.getHeight()-Mouse.getY()-1) / (float)Display.getHeight();
		blurShader.setUniformf("radius", mouseYAmt * MAX_BLUR);

		//draw the horizontally-blurred FBO B to the screen, applying the vertical blur as we go
		batch.draw(blurTargetB, 0, 0);

		batch.end();
	}


		void horizontalBlur() throws LWJGLException {
		//swap the shaders
		//this will send the batch's (FBO-sized) projection matrix to our blur shader
		batch.setShader(blurShader);

		//ensure the direction is along the X-axis only
		blurShader.setUniformf("dir", 1f, 0f);

		//determine radius of blur based on mouse position
		float mouseXAmt = Mouse.getX() / (float)Display.getWidth();
		blurShader.setUniformf("radius", mouseXAmt * MAX_BLUR);

		//start rendering to target B
		blurTargetB.begin();

		//no need to clear since targetA has an opaque background
		//render target A (the scene) using our horizontal blur shader
		//it will be placed into target B
		batch.draw(blurTargetA, 0, 0);

		//flush the batch before ending target B
		batch.flush();

		//finish rendering target B
		blurTargetB.end();

