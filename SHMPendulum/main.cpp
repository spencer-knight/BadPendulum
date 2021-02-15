#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <math.h>
#include <iostream>

float g = 9.8;
struct Circle {

	int radius;
	olc::vf2d pos;
	float mass;
	olc::vf2d acc;
	olc::vf2d vel;
};
class Simulator : public olc::PixelGameEngine
{
public:
	Simulator()
	{
		sAppName = "Example";
	}

public:
	Circle circle;
	float hSub0 = 10;
	float theta = 0;
	float stringLen = 100;//pixels

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		theta = std::acos(1 - (hSub0 / stringLen)); //radians

		circle.pos.x = (ScreenWidth() / 2) + (stringLen * std::sin(theta));
		circle.pos.y = stringLen - hSub0;
		circle.radius = 10;
		circle.mass = 1.f;
		float F_T = 9.8 * circle.mass * std::cos(theta);

		F_T /= circle.mass; //now acceleration
		circle.acc.y = F_T * std::cos(theta) - 9.8f;
		circle.acc.x = F_T * std::sin(theta) * -1.f;

		circle.vel = { 0.f, 0.f };

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		fElapsedTime /= 1;
		// called once per frame
		Clear(olc::BLACK);
		DrawLine(ScreenWidth() / 2, 0, circle.pos.x, circle.pos.y);
		DrawLine(ScreenWidth() / 2, 0, ScreenWidth() / 2, stringLen, olc::VERY_DARK_GREY);//Equilibrium string
		FillCircle(circle.pos, circle.radius);
		FillCircle((int)(ScreenWidth() / 2), (int)stringLen, circle.radius, olc::VERY_DARK_GREY); //Equilibrium circle

		//circle.pos.x += ((circle.vel.x * fElapsedTime) + (0.5f * circle.acc.x * fElapsedTime * fElapsedTime));
		//circle.pos.y -= ((circle.vel.y * fElapsedTime) + (0.5f * circle.acc.y * fElapsedTime * fElapsedTime));

		float velocity = std::sqrt((circle.vel.y * circle.vel.y) + (circle.vel.x * circle.vel.x));
		if (circle.vel.x > 0) {

			velocity *= -1.f;
		}
		float deltaPos = velocity * fElapsedTime;
		circle.pos.x -= deltaPos * std::cos(theta);
		circle.pos.y += deltaPos * std::sin(theta);


		circle.vel.x += circle.acc.x * fElapsedTime * 0.8f;
		circle.vel.y += circle.acc.y * fElapsedTime * 0.8f;
		 

		float x = circle.pos.x - (ScreenWidth() / 2);
		theta = std::atan(x / stringLen);
		//float F_T = ((circle.vel.x * circle.vel.x) + (circle.vel.y * circle.vel.y)) / stringLen;
		float F_T = g * circle.mass * std::cos(theta);
		F_T /= circle.mass;

		circle.acc.y = ((F_T * std::cos(theta)) - g)/circle.mass;
		circle.acc.x = (F_T * std::sin(theta) * -1.f)/circle.mass;
		//circle.acc.y = (circle.pos.x > (ScreenWidth() / 2)) ? 1.f * circle.acc.y : 1.f * circle.acc.y;

		if (theta > -0.05 && theta < 0.05) {

			//circle.vel.y = 0;
		}
		std::cout << circle.acc.x << std::endl;

		return true;
	}
};


int main()
{
	Simulator sim;
	if (sim.Construct(256, 240, 4, 4))
		sim.Start();

	return 0;
}