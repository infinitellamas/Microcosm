// Microclasm.cpp : Defines the entry point for the console application.
//
#include <thread>
#include <future>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
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
	std::vector<Entity*> entities;
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


	/* Ground Entity, used as the floor of the vien */
	Entity ground;
	ground.setGlyphset(createRandomLine(rndrMgr.getWindowX()));
	ground.setPos(b2Vec3(rndrMgr.getWindowX() / 2.0f, rndrMgr.getWindowY() - 1.0f, 0.0f));
	physMgr.addRigidBody(&ground, b2_staticBody, rndrMgr.getWindowX() + 0.0f, 0.5f);
	entities.push_back(&ground);

	/* Rof Entity, used as the roof of the vien */
	Entity roof;
	roof.setGlyphset(createRandomLine(rndrMgr.getWindowX()));
	roof.setPos(b2Vec3(rndrMgr.getWindowX() / 2.0f, 1.0f, 0.0f));
	physMgr.addRigidBody(&roof, b2_staticBody, rndrMgr.getWindowX() + 0.0f, 0.5f);
	entities.push_back(&roof);

	/* Player Entity */
	Entity player;
	player.setGlyphset(pMove1);
	player.setPos({ 15.0f,15.0f, 0.0f });
	physMgr.addRigidBody(&player, b2_dynamicBody, 3, 3);
	physMgr.getRigidBodyPtr(&player)->SetLinearDamping(1000.0f);
	b2Filter playerFilter;
	playerFilter.groupIndex = -2;
	physMgr.getRigidBodyPtr(&player)->GetFixtureList()[0].SetFilterData(playerFilter);
	entities.push_back(&player);

	// Create some particles to act as if the virus is moving forward
	unsigned int numParticles = 20;
	std::vector<Entity> particles;
	for (unsigned int i = 0; i < numParticles; i++) {
		Entity particle;
		particle.setGlyphset({ Glyph('.',b2Vec2(0.0f,0.0f)) });
		particle.setPos(b2Vec3((rand() % rndrMgr.getWindowX()) + 0.0f, (rand() % rndrMgr.getWindowY()) + 0.0f, -5.0f)); // -1 should render behind the player
		particles.push_back(particle);
	}
	 
	Glyphset bulletGS = { Glyph('-', b2Vec2(-1,0)),Glyph('=', b2Vec2(0,0)) ,Glyph('<', b2Vec2(1,0)) };
	Entity bullet1;
	bullet1.setGlyphset(bulletGS);
	bullet1.setPos(b2Vec3(-1.0f,-1.0f,1.0f));
	physMgr.addRigidBody(&bullet1, b2_dynamicBody, 1.5f, 0.5f);
	physMgr.getRigidBodyPtr(&bullet1)->GetFixtureList()[0].SetFilterData(playerFilter);
	bool bullet1State = false; //False means not shot, true means shot.
	entities.push_back(&bullet1);

	/* Listen for keys being pressed.*/
	inptMgr.listenForKey('q');
	inptMgr.listenForKey('w');
	inptMgr.listenForKey('a');
	inptMgr.listenForKey('d');
	inptMgr.listenForKey('s');
	inptMgr.listenForKey('p');

	/* Some player-specific variables so i can update the animations based on the players location*/
	int lastPlayerX = 0;
	int lastPlayerY = 0;

	/* Variables for time-based events*/
	std::chrono::milliseconds lastQuarterSecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::milliseconds lastSecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::milliseconds thisSecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	std::chrono::microseconds lastMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::microseconds thisMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	
	while (!inptMgr.getKey('q').getKeyDown()) {
		std::this_thread::sleep_for((std::chrono::milliseconds)(1));
		thisMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		if (lastMilliseconds + (std::chrono::milliseconds)(1000/60) < thisMilliseconds) {
			lastMilliseconds = thisMilliseconds;

			// Update inputs and step physics
			inptMgr.updateKeys();
			physMgr.stepPhysics();

			//Do some time-based event handling
			thisSecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

			// Do this every quarter second
			if ((lastQuarterSecond + (std::chrono::milliseconds)250) < thisSecond) {
				// Changine the roof and ground Glyphset
				roof.setGlyphset(createRandomLine(rndrMgr.getWindowX()));
				ground.setGlyphset(createRandomLine(rndrMgr.getWindowX()));
				lastQuarterSecond = thisSecond;
			}

			/* Update player */
			{
				if (round(lastPlayerX) < round(player.getPos().x)) {
					movePlayerAnim(&player, true); // true = right
					lastPlayerX = round(player.getPos().x) + 0.0f;
				}
				else if (round(lastPlayerX) > round(player.getPos().x)) {
					movePlayerAnim(&player, false); // true = right
					lastPlayerX = round(player.getPos().x) + 0.0f;
				}

				float force = 0.5f;
				if (inptMgr.getKey('w').getKeyHeld()) {
					physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(0, -force), true);
				}
				if (inptMgr.getKey('s').getKeyHeld()) {
					physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(0, force), true);
				}
				if (inptMgr.getKey('a').getKeyHeld()) {
					physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(-force, 0), true);
				}
				if (inptMgr.getKey('d').getKeyHeld()) {
					physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(force, 0), true);
				}
				if (inptMgr.getKey('p').getKeyDown()) {
					bullet1.setPos(b2Vec3(player.getPos().x, player.getPos().y, bullet1.getPos().z));
					//physMgr.getRigidBodyPtr(&bullet1)->ApplyLinearImpulse(b2Vec2(1.0f, 0.0f), b2Vec2(0.0f,0.0f), true);
				}
			}


			/* Update particle effects */
			for (unsigned int i = 0; i < numParticles; i++) {
				//Give the particles some random motion
				particles.at(i).setPos(particles.at(i).getPos() + b2Vec3(-((float)i + 1) / 4, 0, 0));
				if (particles.at(i).getPos().x < -2) {
					particles.at(i).setPos(b2Vec3(rndrMgr.getWindowX() + 2, (rand() % rndrMgr.getWindowY()), -5.0f));
				}
			}

			std::vector<Entity*> entitiesThisFrame;
			entitiesThisFrame.reserve(entities.size() + particles.size());
			entitiesThisFrame.insert(entitiesThisFrame.end(), entities.begin(), entities.end());
			// This has to be done every frame because the pointers could become invalid.
			for (unsigned int i = 0; i < particles.size(); i++) {
				entitiesThisFrame.push_back(&(particles.at(i)));
			}

			rndrMgr.RenderScene(entitiesThisFrame);
			entitiesThisFrame.clear();
		}
	}

	system("PAUSE");
    return 0;
}

