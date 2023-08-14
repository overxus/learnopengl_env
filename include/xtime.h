#pragma once


namespace xTime {
	float deltaTime = 1 / 30.0f;

	void setFrameRate(float frame_rate = 30.0f) {
		if (frame_rate != 0) {
			deltaTime = 1.0f / frame_rate;
		}
	}
}