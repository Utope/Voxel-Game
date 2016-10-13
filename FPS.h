
#define FPS_MAX 60
#include "SDL\SDL.h"

struct FpsData
{
	int fps;
	float max_frame_ticks;
	unsigned int frame_min;
	unsigned int frame_max;
	double average;
};

class FPS {

public:

	FPS() {};
	~FPS() {};

	float max_frame_ticks;
	unsigned int last_second_ticks;
	int frame_count;

	unsigned int min_ticks;
	unsigned int max_ticks;
	double averageticks;
	unsigned int last_frame_ticks;

	unsigned int FPS::current_ticks;
	unsigned int target_ticks;
	FpsData fpsData;

	void FPS::init() {
		max_frame_ticks = (1000.0f / (float)FPS_MAX + 0.00001f);
		frame_count = 0;
		last_second_ticks = SDL_GetTicks();
	}

	bool limit() {
		frame_count++;
		target_ticks = last_second_ticks + unsigned int(frame_count * max_frame_ticks);
		current_ticks = SDL_GetTicks();

		averageticks += current_ticks - last_frame_ticks;
		
		if (current_ticks - last_frame_ticks) {
			min_ticks = current_ticks - last_frame_ticks;
		}

		if (current_ticks - last_frame_ticks >= max_frame_ticks) {
			max_ticks = current_ticks - last_frame_ticks;
		}

		if (current_ticks < target_ticks) {
			SDL_Delay(target_ticks - current_ticks);
			current_ticks = SDL_GetTicks();
		}

		last_frame_ticks = current_ticks;

		if (current_ticks - last_second_ticks >= 1000) {
			fpsData.fps = frame_count;
			fpsData.average = averageticks / frame_count;
			fpsData.frame_min = min_ticks;
			fpsData.frame_max = max_ticks;

			frame_count = 0;
			min_ticks = 1000;
			max_ticks = 0;
			averageticks = 0;
			last_second_ticks = SDL_GetTicks();
			return true;
		}

		return false;
	}

private:
	
};

