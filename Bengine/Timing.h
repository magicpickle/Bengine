#pragma once

namespace Bengine
{
	class FpsLimiter
	{
	public:
		FpsLimiter();
		void init(float maxFps);

		void setMaxFps(float maxFps);

		void begin();

		//return the current fps
		float end();
	private:
		void calculateFPS();

		float _fps;
		float _frameTime;
		float _maxFPS;
		
		unsigned int _startTicks;
	};
}