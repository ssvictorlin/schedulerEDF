#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
	int i;
	// You can initialize the shared variable if needed.
	sv->bProgramExit = 0;
	//sv->ign = 0;
	sv->preButton = 0;
	sv->tempAlarm = 0;
	sv->trackAlarm = 0;
	sv->touchCounter = 8;
	sv->rgbState = 0;
	for (i = 0; i < 8; i++) sv->counter[i] = 0;
}

void init_sensors(SharedVariable* sv) {
	pinMode(PIN_BUTTON, INPUT);
	pinMode(PIN_YELLOW, OUTPUT);
	pinMode(PIN_TEMP, INPUT);
	pinMode(PIN_TRACK, INPUT);
	pinMode(PIN_TOUCH, INPUT);
	pinMode(PIN_RED, OUTPUT);
	pinMode(PIN_GREEN, OUTPUT);
	pinMode(PIN_BLUE, OUTPUT);
	pinMode(PIN_ALED, OUTPUT);
	pinMode(PIN_BUZZER, OUTPUT);
}

void body_button(SharedVariable* sv) {
	int button = digitalRead (PIN_BUTTON);
	if (sv->preButton == HIGH && button == LOW){
		sv->rgbState = (sv->rgbState != 0)? 0:1;	
	}
	sv->preButton = button;
}

void body_twocolor(SharedVariable* sv) {
	if (sv->rgbState == 0) {
		digitalWrite (PIN_YELLOW, 0);		
	} else {
		digitalWrite (PIN_YELLOW, 255);
	}
}

void body_temp(SharedVariable* sv) {
	if (sv->rgbState == 1) {
		int temperature = digitalRead (PIN_TEMP);
		//printf("temp is now: %d\n", temperature);
		if (temperature == HIGH) {
			sv->tempAlarm = 1;
		}
		else {
			sv->tempAlarm = 0;
		}	
	} else {
		sv->tempAlarm = 0;
	}	
}

void body_track(SharedVariable* sv) {
	if (sv->rgbState == 0) {
		//reset to 0
		sv->trackAlarm = 0;
	} else {
		int track = digitalRead (PIN_TRACK);
		//printf("track is now: %d\n", track);
		if (track == LOW) {
			sv->trackAlarm = 1;
			sv->rgbState = 2;
		}
	}
}

void body_touch(SharedVariable* sv) {
	if (sv->rgbState == 0) {
		//reset to 0
		sv->touchAlarm = 0;
	} else {
		int touch = digitalRead (PIN_TOUCH);
		//printf("touch is now: %d\n", touch);
		if (touch == HIGH) {
			sv->touchAlarm = 1;
			sv->rgbState = 3;
			sv->touchCounter = 8;
		}
		else {
			if (sv->touchCounter-- > 0) return;	
			sv->touchAlarm = 0;
		}
	}
}

void body_rgbcolor(SharedVariable* sv) {
	if (sv->rgbState == 0) {
		digitalWrite (PIN_RED, 0);
		digitalWrite (PIN_GREEN, 0);
		digitalWrite (PIN_BLUE, 255);
	} else 	if (sv->rgbState == 1) {
		digitalWrite (PIN_RED, 255);
		digitalWrite (PIN_GREEN, 255);
		digitalWrite (PIN_BLUE, 0);
	} else if (sv->rgbState == 2) {
		digitalWrite (PIN_RED, 255);
		digitalWrite (PIN_GREEN, 0);
		digitalWrite (PIN_BLUE, 0);
	} else {
		digitalWrite (PIN_RED, 200);
		digitalWrite (PIN_GREEN, 59);
		digitalWrite (PIN_BLUE, 255);
	}
}

void body_aled(SharedVariable* sv) {
	if (!sv->tempAlarm) {
		digitalWrite (PIN_ALED, 0);
	} else {
		digitalWrite (PIN_ALED, 1);
	}
}

void body_buzzer(SharedVariable* sv) {
	if (!sv->touchAlarm) {
		digitalWrite (PIN_BUZZER, 0);
	} else {
		digitalWrite (PIN_BUZZER, 1);
	}
}
