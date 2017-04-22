// Microclasm.cpp : Defines the entry point for the console application.
//
#include <thread>
#include <future>
#include <vector>
#include <iostream>
#include "Windows.h"
#include "SoundManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "Glyph.h"
#include "Entity.h"

#include "GameData.h"


int main()
{
	// Init all the things...
	RenderManager rndrMgr;
	SoundManager sndMgr;
	PhysicsManager physMgr;
	InputManager inptMgr;
	
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 20;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), (ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));


	/* Ground Entity, used as the floor of the vien */
	Entity ground;
	Glyphset groundSet;
	for (int i = 0; i < rndrMgr.getWindowX()+2; i++) {
		groundSet.push_back(Glyph('-', b2Vec2(-(rndrMgr.getWindowX() + 2)/2 + i,0.0f)));
	}
	ground.setGlyphset(groundSet);
	ground.setPos(b2Vec3(rndrMgr.getWindowX() / 2, rndrMgr.getWindowY()+1, 0));
	physMgr.addRigidBody(&ground, b2_staticBody, 10, 0.5);

	/* Rof Entity, used as the roof of the vien */
	Entity roof;
	Glyphset roofSet;
	for (int i = 0; i < rndrMgr.getWindowX() + 2; i++) {
		roofSet.push_back(Glyph('-', b2Vec2(-(rndrMgr.getWindowX() + 2)/2 + i, 0.0f)));
	}
	roof.setGlyphset(roofSet);
	roof.setPos(b2Vec3(rndrMgr.getWindowX() / 2, -1, 0));

	/* Player Entity */
	Entity player;
	player.setGlyphset(pMove1);
	player.setPos({ 15.0f,15.0f, 0.0f });
	physMgr.addRigidBody(&player, b2_dynamicBody, 3, 3);
	physMgr.getRigidBodyPtr(&player)->SetLinearDamping(0.1f);


	inptMgr.listenForKey('q');
	inptMgr.listenForKey('w');
	inptMgr.listenForKey('a');
	inptMgr.listenForKey('d');
	inptMgr.listenForKey('s');

	int lastPlayerX = 0;
	int lastPlayerY = 0;

	std::chrono::milliseconds lastSecond = std::chrono::duration_cast<  std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::milliseconds thisSecond = std::chrono::duration_cast<  std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

	while (!inptMgr.getKey('q').getKeyDown()) {
		// Update inputs and step physics
		inptMgr.updateKeys();
		physMgr.stepPhysics();

		//Do some time-based event handling
		thisSecond = std::chrono::duration_cast<  std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		if ((lastSecond + (std::chrono::milliseconds)1000) < thisSecond) {
			//do something
		}
		

		if (round(lastPlayerX) < round(player.getPos().x)) {
			movePlayerAnim(&player, true); // true = right
			lastPlayerX = round(player.getPos().x);
		}
		else if (round(lastPlayerX) > round(player.getPos().x)) {
			movePlayerAnim(&player, false); // true = right
			lastPlayerX = round(player.getPos().x);
		}

		float impulse = 0.0005f;

		if (inptMgr.getKey('w').getKeyDown()) {
			physMgr.getRigidBodyPtr(&player)->ApplyLinearImpulse(b2Vec2(0, -impulse), b2Vec2(0, 0), true);
		}
		if (inptMgr.getKey('s').getKeyDown()) {
			physMgr.getRigidBodyPtr(&player)->ApplyLinearImpulse(b2Vec2(0, impulse), b2Vec2(0, 0), true);
		}
		if (inptMgr.getKey('a').getKeyDown()) {
			physMgr.getRigidBodyPtr(&player)->ApplyLinearImpulse(b2Vec2(-impulse, 0), b2Vec2(0, 0), true);
		}
		if (inptMgr.getKey('d').getKeyDown()) {
			physMgr.getRigidBodyPtr(&player)->ApplyLinearImpulse(b2Vec2(impulse, 0), b2Vec2(0, 0), true);
		}


		if (inptMgr.getKey('w').getKeyHeld()) {
			physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(0, -0.005f), true);
		}
		if (inptMgr.getKey('s').getKeyHeld()) {
			physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(0, 0.005f), true);
		}
		if (inptMgr.getKey('a').getKeyHeld()) {
			physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(-0.005f, 0), true);
		}
		if (inptMgr.getKey('d').getKeyHeld()) {
			physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(0.005f, 0), true);
		}


		rndrMgr.RenderScene({ player, ground, roof });
	}

	system("PAUSE");
    return 0;
}

