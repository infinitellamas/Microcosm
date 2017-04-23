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

std::pair<int, int> runGame(); //Returns game time and win state
bool showMenu(int gotoState, int gameTime); //true if continue, false if quit

RenderManager rndrMgr;
SoundManager sndMgr;
PhysicsManager physMgr;
InputManager inptMgr;

char up = 'w',down = 's',left = 'a',right = 'd',shoot = 'p',absorb = 'o';


int main()
{

	sndMgr.addSound("assets/sounds/game_bgm.wav");
	sndMgr.addSound("assets/sounds/menu_bgm.wav");
	sndMgr.addSound("assets/sounds/death_sfx.wav");
	sndMgr.addSound("assets/sounds/player_hit_sfx.wav");
	sndMgr.addSound("assets/sounds/player_shoot_sfx.wav");
	sndMgr.addSound("assets/sounds/redcell_absorb_finish_sfx.wav");
	sndMgr.addSound("assets/sounds/redcell_absorb_sfx.wav");
	sndMgr.addSound("assets/sounds/whitecell_hit_sfx.wav");
	sndMgr.addSound("assets/sounds/whitecell_shoot_sfx.wav");


	std::pair<int,int> gameInfo(-1,-1);
	while (showMenu(gameInfo.first, gameInfo.second)) {
		
		// I know this is akin to heresy but I dont have much time
		//(&rndrMgr)->~RenderManager();
		//new (&rndrMgr) RenderManager();

		//(&sndMgr)->~SoundManager();
		//new (&sndMgr) SoundManager();

		(&physMgr)->~PhysicsManager();
		new (&physMgr) PhysicsManager();

		(&inptMgr)->~InputManager();
		new (&inptMgr) InputManager();



		gameInfo = runGame();
	}


    return 0;
}

bool showMenu(int gotoState, int gameTime) {
	int menustate = 0;
	if (gotoState == -1) { // We are beginning
		menustate = 0;
	}
	if (gotoState == 0) { // We just lost
		menustate = 2;
	}
	if (gotoState == 1) { // we just won
		menustate = 3;
	}

	inptMgr.listenForKey(up);
	inptMgr.listenForKey('z'); //azerty up
	inptMgr.listenForKey('q'); //qwertz left
	inptMgr.listenForKey(down);
	inptMgr.listenForKey(shoot);
	inptMgr.listenForKey(absorb);
	int SelectorPos = 0;

	/*
		0 is main menu, 1 is tutorial and controls, 2 is win screen, 3 is lose screen
	*/

	HCHANNEL ch = sndMgr.playSound("assets/sounds/menu_bgm.wav");
	BASS_ChannelPlay(ch, FALSE);
	std::chrono::milliseconds thisMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::milliseconds lastMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	int menuSoundMilliseconds = 10100;
	std::chrono::milliseconds timeSoundPlayed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	while (true) {
		if (timeSoundPlayed.count() + menuSoundMilliseconds < thisMilliseconds.count()) {
			timeSoundPlayed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
			
			BASS_ChannelStop(ch);
			BASS_ChannelPlay(ch = sndMgr.playSound("assets/sounds/menu_bgm.wav"), FALSE);
		}
		
		
		std::this_thread::sleep_for((std::chrono::milliseconds)(10));
		thisMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		if (lastMilliseconds.count() + 1 / 60 < thisMilliseconds.count()) {
			lastMilliseconds = thisMilliseconds;
			inptMgr.updateKeys();
			if (menustate == 0) {

				Entity Title;
				Glyphset TitleSet = stringToGlyphset("Microcosm", { 0,0 });
				Title.setGlyphset(TitleSet);
				Title.setPos({ 5,2,0 });

				Entity Subtitle;
				Glyphset SubtitleSet = stringToGlyphset("noun. a little world; a world in miniature", { 0,0 });
				Subtitle.setGlyphset(SubtitleSet);
				Subtitle.setPos({ 5,3,0 });

				Entity PlayerRep;
				std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
				if ((time.count() % 1000) < 250) {
					PlayerRep.setGlyphset(pMove1);
				}
				else if ((time.count() % 1000) < 500) {
					PlayerRep.setGlyphset(pMove2);
				}
				else if ((time.count() % 1000) < 750) {
					PlayerRep.setGlyphset(pMove3);
				}
				else {
					PlayerRep.setGlyphset(pMove4);
				}
				PlayerRep.setPos({ (float)(rndrMgr.getWindowX() - 20),(float)(rndrMgr.getWindowY() / 2) , 0.0f });


				Entity PlayGame;
				Glyphset PlayGameSet = stringToGlyphset("Play Game", { 0,0 });
				PlayGame.setGlyphset(PlayGameSet);
				PlayGame.setPos({ 7,12,0 });

				Entity SetControls;
				Glyphset SetControlsSet = stringToGlyphset("Tutorial", { 0,0 });
				SetControls.setGlyphset(SetControlsSet);
				SetControls.setPos({ 7,14,0 });

				Entity Quit;
				Glyphset QuitSet = stringToGlyphset("Quit", { 0,0 });
				Quit.setGlyphset(QuitSet);
				Quit.setPos({ 7,16,0 });

				Entity Selector;
				Glyphset SelectorSet = { Glyph('>',{0,0}) };
				Selector.setGlyphset(SelectorSet);
				Selector.setPos({ 6,(float)(12 + SelectorPos * 2),0 });
				HCHANNEL m;
				if (inptMgr.getKey(down).getKeyDown()) {
					SelectorPos++;
					BASS_ChannelPlay(m = sndMgr.playSound("assets/sounds/whitecell_hit_sfx.wav"), FALSE);
					if (SelectorPos > 2) {
						SelectorPos = 0;
					}
				}
				if (inptMgr.getKey(up).getKeyDown() || inptMgr.getKey('z').getKeyDown()) {
					SelectorPos--;
					BASS_ChannelPlay(m = sndMgr.playSound("assets/sounds/whitecell_hit_sfx.wav"), FALSE);
					if (SelectorPos < 0) {
						SelectorPos = 2;
					}
				}
				if (inptMgr.getKey(shoot).getKeyDown()) {
					if (SelectorPos == 0) {
						BASS_ChannelStop(ch);


						return true; //Play the game
					}
					if (SelectorPos == 1) {
						menustate = 1;
					}
					if (SelectorPos == 2) {
						return false;
					}

				}

				Entity currentControlScheme;
				std::stringstream k;
				k << "Controls: " << up << "/" << down << "/" << left << "/" << right << " shoot: " << shoot << " absorb: " << absorb;
				Glyphset currentControlSchemeSet = stringToGlyphset(k.str(), b2Vec2(0, 0));
				currentControlScheme.setGlyphset(currentControlSchemeSet);
				currentControlScheme.setPos({ 5,(float)rndrMgr.getWindowY() - 2,0 });

				

				rndrMgr.setWobble(true, 1);
				rndrMgr.RenderScene({ &Title, &Subtitle, &PlayerRep, &PlayGame, &SetControls, &Quit, &Selector, &currentControlScheme });


			}
			if (menustate == 1) { // Controls and Tutorial
				Entity Title;
				Glyphset TitleSet = stringToGlyphset("Microcosm", { 0,0 });
				Title.setGlyphset(TitleSet);
				Title.setPos({ 5,2,0 });

				Entity Subtitle;
				Glyphset SubtitleSet = stringToGlyphset("Tutorial and Controls", { 0,0 });
				Subtitle.setGlyphset(SubtitleSet);
				Subtitle.setPos({ 5,3,0 });

				Entity Desc1;
				Glyphset Desc1SetFull;
				Glyphset Desc1Set = stringToGlyphset("This virus has entered its first victim and needs", { 0,0 });
				Glyphset Desc1Set2 = stringToGlyphset("your help to become strong enough to take over.", { 0,1 });
				Glyphset Desc1Set3 = stringToGlyphset("Guide the virus and absorb red blood cells while", { 0,2 });
				Glyphset Desc1Set4 = stringToGlyphset("shooting virions to fend off the white blood cell army!", { 0,3 });
				Desc1SetFull.insert(Desc1SetFull.end(), Desc1Set.begin(), Desc1Set.end());
				Desc1SetFull.insert(Desc1SetFull.end(), Desc1Set2.begin(), Desc1Set2.end());
				Desc1SetFull.insert(Desc1SetFull.end(), Desc1Set3.begin(), Desc1Set3.end());
				Desc1SetFull.insert(Desc1SetFull.end(), Desc1Set4.begin(), Desc1Set4.end());
				Desc1.setGlyphset(Desc1SetFull);
				Desc1.setPos({ 5,5,0 });

				Entity currentControlScheme;
				std::stringstream k;
				k << "Controls: " << up << "/" << down << "/" << left << "/" << right << " shoot: " << shoot << " absorb: " << absorb;
				Glyphset currentControlSchemeSet = stringToGlyphset(k.str(), b2Vec2(0, 0));
				currentControlScheme.setGlyphset(currentControlSchemeSet);
				currentControlScheme.setPos({ 5,(float)rndrMgr.getWindowY() - 2,0 });

				Entity PlayerRep;
				std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
				if ((time.count() % 1000) < 250) {
					PlayerRep.setGlyphset(pMove1);
				}
				else if ((time.count() % 1000) < 500) {
					PlayerRep.setGlyphset(pMove2);
				}
				else if ((time.count() % 1000) < 750) {
					PlayerRep.setGlyphset(pMove3);
				}
				else {
					PlayerRep.setGlyphset(pMove4);
				}
				PlayerRep.setPos({ 7,16,0 });

				Entity RedCellRep;
				RedCellRep.setGlyphset(rC1);
				RedCellRep.setPos({ 21,16,0 });

				Entity WhiteCellRep;
				WhiteCellRep.setGlyphset(wC1);
				WhiteCellRep.setPos({ 35,16,0 });

				Entity ImgDesc;
				ImgDesc.setGlyphset(stringToGlyphset("You         Red Cell     White Cell", b2Vec2(0,0)));
				ImgDesc.setPos({ 6,11, 0 });

				Entity backInfo;
				k.str(std::string());
				k.clear();
				k << "Press " << absorb << " to go back.";
				backInfo.setGlyphset(stringToGlyphset(k.str(), b2Vec2(0, 0)));
				backInfo.setPos(b2Vec3(5,(int) rndrMgr.getWindowY() - 1, 0));

				rndrMgr.setWobble(false, 0);
				rndrMgr.RenderScene({ &Title, &Subtitle, &currentControlScheme, &Desc1, &PlayerRep, &RedCellRep, &WhiteCellRep, &ImgDesc, &backInfo });

				
				if (inptMgr.getKey(absorb).getKeyDown()) {
					menustate = 0;
				}
			}
			if (menustate == 2) { // lost
				Entity Title;
				Glyphset TitleSet = stringToGlyphset("Microcosm", { 0,0 });
				Title.setGlyphset(TitleSet);
				Title.setPos({ 5,2,0 });

				Entity PlayerRep;
				std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
				PlayerRep.setGlyphset(wC1);
				
				int movex;
				int movey;
				if (time.count() % 100 < rand() % 10) {
					movex = -2  + rand() % 4;
					movey = -2  + rand() % 4;
				}
				else {
					movex = 0;
					movey = 0;
				}

				PlayerRep.setPos({ (float)(rndrMgr.getWindowX() - 20) + movex,(float)(rndrMgr.getWindowY() / 2) + movey , 0.0f });

				Entity gameInfo;
				std::stringstream k;
				Glyphset gameInfo1 = stringToGlyphset("YOU LOST!", { 0,0 });
				k << "Your virus lasted a whole " << gameTime << " seconds before being destroyed.";
				Glyphset gameInfo2 = stringToGlyphset(k.str(), { 0,1 });
				Glyphset gameInfo3 = stringToGlyphset("Better luck next time!", { 0,3 });
				Glyphset gameInfoFull;
				gameInfoFull.insert(gameInfoFull.begin(), gameInfo1.begin(), gameInfo1.end());
				gameInfoFull.insert(gameInfoFull.begin(), gameInfo2.begin(), gameInfo2.end());
				gameInfoFull.insert(gameInfoFull.begin(), gameInfo3.begin(), gameInfo3.end());
				gameInfo.setGlyphset(gameInfoFull);
				gameInfo.setPos({ 5,5,0 });

				
				Entity backInfo;
				k.str(std::string());
				k.clear();
				k << "Press " << absorb << " to go back to the menu.";
				backInfo.setGlyphset(stringToGlyphset(k.str(), b2Vec2(0, 0)));
				backInfo.setPos(b2Vec3(5, (int)rndrMgr.getWindowY() - 1, 0));

				rndrMgr.setWobble(false, 0);
				rndrMgr.RenderScene({ &Title,  &PlayerRep, &gameInfo, &backInfo });


				if (inptMgr.getKey(absorb).getKeyDown()) {
					menustate = 0;
				}

			}
			if (menustate == 3) { //won
				Entity Title;
				Glyphset TitleSet = stringToGlyphset("Microcosm", { 0,0 });
				Title.setGlyphset(TitleSet);
				Title.setPos({ 5,2,0 });

				Entity PlayerRep;
				std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
				if ((time.count() % 500) < 125) {
					PlayerRep.setGlyphset(pMove1);
				}
				else if ((time.count() % 500) < 250) {
					PlayerRep.setGlyphset(pMove2);
				}
				else if ((time.count() % 500) < 375) {
					PlayerRep.setGlyphset(pMove3);
				}
				else {
					PlayerRep.setGlyphset(pMove4);
				}
				PlayerRep.setPos({ (float)(rndrMgr.getWindowX() - 20),(float)(rndrMgr.getWindowY() / 2) , 0.0f });


				Entity gameInfo;
				std::stringstream k;
				Glyphset gameInfo1 = stringToGlyphset("YOU WON!", { 0,0 });
				k << "Your virus lasted a whole " << gameTime << " seconds before defeating the immune system.";
				Glyphset gameInfo2 = stringToGlyphset(k.str(), { 0,1 });
				Glyphset gameInfo3 = stringToGlyphset("Your virus went on to infect the rest of humanity.", { 0,3 });
				Glyphset gameInfo4 = stringToGlyphset("There were no survivors.", { 0,4 });
				Glyphset gameInfo5 = stringToGlyphset("I hope you are happy.", { 0,5 });
				Glyphset gameInfoFull;
				gameInfoFull.insert(gameInfoFull.begin(), gameInfo1.begin(), gameInfo1.end());
				gameInfoFull.insert(gameInfoFull.begin(), gameInfo2.begin(), gameInfo2.end());
				gameInfoFull.insert(gameInfoFull.begin(), gameInfo3.begin(), gameInfo3.end());
				gameInfoFull.insert(gameInfoFull.begin(), gameInfo4.begin(), gameInfo4.end());
				gameInfoFull.insert(gameInfoFull.begin(), gameInfo5.begin(), gameInfo5.end());
				gameInfo.setGlyphset(gameInfoFull);
				gameInfo.setPos({ 5,5,0 });

				Entity backInfo;
				k.str(std::string());
				k.clear();
				k << "Press " << absorb << " to go back to the menu.";
				backInfo.setGlyphset(stringToGlyphset(k.str(), b2Vec2(0, 0)));
				backInfo.setPos(b2Vec3(5, (int)rndrMgr.getWindowY() - 1, 0));

				rndrMgr.setWobble(false, 0);
				rndrMgr.RenderScene({ &Title, &PlayerRep, &gameInfo, &backInfo });


				if (inptMgr.getKey(absorb).getKeyDown()) {
					menustate = 0;
				}
			}
		}
	}
}

std::pair<int,int> runGame() {
	int gameTime = 0; //Survival time in seconds
	int winState = 0; //0 is win, 1 is lose.

	HCHANNEL bgmChannel = sndMgr.playSound("assets/sounds/game_bgm.wav");
	BASS_ChannelPlay(bgmChannel, FALSE);
	HCHANNEL ch2;

	std::vector<Entity*> entities;
	// Init all the things...
	
	ContactListener cntctLstnr;
	physMgr.getWorld()->SetContactListener(&cntctLstnr);

	/* Ground Entity, used as the floor of the vien */
	Entity ground;
	ground.setGlyphset(createRandomLine(rndrMgr.getWindowX()));
	ground.setPos(b2Vec3(rndrMgr.getWindowX() / 2.0f, rndrMgr.getWindowY() - 1.0f, 0.0f));
	physMgr.addRigidBody(&ground, b2_staticBody, rndrMgr.getWindowX() + 0.0f, 0.5f);
	entities.push_back(&ground);

	/* Rof Entity, used as the roof of the vien */
	Entity roof;
	roof.setGlyphset(createRandomLine(rndrMgr.getWindowX()));
	roof.setPos(b2Vec3(rndrMgr.getWindowX() / 2.0f, 2.0f, 0.0f));
	physMgr.addRigidBody(&roof, b2_staticBody, rndrMgr.getWindowX() + 0.0f, 0.5f);
	entities.push_back(&roof);

	/* Player Entity */
	Player player;
	player.setGlyphset(pMove1);
	player.setPos({ 15.0f,15.0f, 0.0f });
	physMgr.addRigidBody(&player, b2_dynamicBody, 3, 3);
	physMgr.getRigidBodyPtr(&player)->SetLinearDamping(1000.0f);
	entities.push_back(&player);

	Entity suction;
	suction.setGlyphset(suction1);
	suction.setPos({ -20.0,-20.0, 0.25f }); // In between the virus and the cell
	entities.push_back(&suction);
	int suctionFrame = 1;
	int suctionCooldown = 0;
	int suctionCooldownMax = 5;

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
	Bullet bulletTemplate;
	bulletTemplate.setGlyphset(bulletGS);
	bulletTemplate.setPos(b2Vec3(-20.0f, -20.0f, 1.0f));
	//physMgr.addRigidBody(&bulletTemplate, b2_dynamicBody, 1.5f, 0.5f);
	b2Filter playerFilter;
	playerFilter.maskBits = 0x0002; // Set enemies category to 0x0002
	b2Filter enemyFilter;
	enemyFilter.categoryBits = 0x0002; // Set enemies category to 0x0002

	std::vector<Bullet*> bullets;
	int bulletCooldownMax = 30;
	int bulletCooldown = 0;

	std::vector<RedCell*> redCells;

	std::vector<WhiteCell*> whiteCells;

	Glyphset enemyBulletGS = { Glyph('>', b2Vec2(-1,0)),Glyph('=', b2Vec2(0,0)) ,Glyph('-', b2Vec2(1,0)) };
	Bullet enemyBulletTemplate;
	enemyBulletTemplate.setGlyphset(enemyBulletGS);
	enemyBulletTemplate.setPos(b2Vec3(-20.0f, -20.0f, 1.0f));
	//physMgr.addRigidBody(&enemyBulletTemplate, b2_dynamicBody, 1.5f, 0.5f);
	b2Filter enemyBulletFilter;
	enemyBulletFilter.maskBits = ~0x0002; // Dont collide with enemies
	std::vector<Bullet*> enemyBullets;

	entities.push_back(&enemyBulletTemplate);


	/* Listen for keys being pressed.*/
	inptMgr.listenForKey(up);
	inptMgr.listenForKey('z');
	inptMgr.listenForKey('q');
	inptMgr.listenForKey(down);
	inptMgr.listenForKey(left);
	inptMgr.listenForKey(right);
	inptMgr.listenForKey(shoot);
	inptMgr.listenForKey(absorb);

	/* Some player-specific variables so i can update the animations based on the players location*/
	int lastPlayerX = 0;
	int lastPlayerY = 0;
	int absorbCooldown = 0;
	int absorbCooldownMax = 20;

	float absorbtions = 20;
	int totalAbsorptions = 100;

	Entity* healthBar = new Entity;
	healthBar->setPos({ (float)(rndrMgr.getWindowX()) / 2, 0, 5.0f }); // In front of stage
	Glyphset healthBarGS;
	for (int i = 0; i < rndrMgr.getWindowX(); i++) {
		if ((i*totalAbsorptions) / rndrMgr.getWindowX() < absorbtions) {
			healthBarGS.push_back(Glyph('>', { (float)(i - (rndrMgr.getWindowX() / 2)), 0 }));
		}
		else {
			healthBarGS.push_back(Glyph('_', { (float)(i - (rndrMgr.getWindowX() / 2)), 0 }));
		}
	}
	healthBar->setGlyphset(healthBarGS);
	entities.push_back(healthBar);
	int framesPerHealthMax = 10;
	int framesPerHealth = framesPerHealthMax;

	std::chrono::milliseconds beginGame = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());


	/* Variables for time-based events*/
	std::chrono::milliseconds lastQuarterSecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::milliseconds lastSecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::milliseconds thisSecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	std::chrono::microseconds lastMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::chrono::microseconds thisMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	long long loops = 0;
	int bgmMillis = 8700;
	std::chrono::milliseconds lastSongLoop = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	while (absorbtions > 0 && absorbtions < totalAbsorptions) {
		thisMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		if (lastSongLoop.count() + bgmMillis < thisSecond.count()) {
			lastSongLoop = thisSecond;
			BASS_ChannelStop(bgmChannel);
			BASS_ChannelPlay(bgmChannel = sndMgr.playSound("assets/sounds/game_bgm.wav") , FALSE);
		}




		std::this_thread::sleep_for((std::chrono::milliseconds)(1));
		if (lastMilliseconds + (std::chrono::milliseconds)((int)(1000 / (60+(40*absorbtions/100)))) < thisMilliseconds) {
			loops++;
			lastMilliseconds = thisMilliseconds;
			std::vector<Entity*> entitiesThisFrame;

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
				if (inptMgr.getKey(up).getKeyHeld() || inptMgr.getKey('z').getKeyHeld()) {
					physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(0, -force), true);
				}
				if (inptMgr.getKey(down).getKeyHeld()) {
					physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(0, force), true);
				}
				if (inptMgr.getKey(left).getKeyHeld() || inptMgr.getKey('q').getKeyHeld()) {
					physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(-force, 0), true);
				}
				if (inptMgr.getKey(right).getKeyHeld()) {
					physMgr.getRigidBodyPtr(&player)->ApplyForceToCenter(b2Vec2(force, 0), true);
				}
				if (inptMgr.getKey(shoot).getKeyDown()) {
					//Spawn a bullet
					absorbtions--;
					Bullet* bullet = new Bullet;
					bullet->setGlyphset(bulletGS);
					physMgr.addRigidBody(bullet, b2_dynamicBody, 1.5f, 0.5f);
					physMgr.getRigidBodyPtr(bullet)->GetFixtureList()[0].SetFilterData(playerFilter);

					physMgr.getRigidBodyPtr(bullet)->SetTransform({ player.getPos().x / 100, player.getPos().y / 100 }, 0);
					physMgr.getRigidBodyPtr(bullet)->ApplyLinearImpulse(b2Vec2(0.0001f, 0.0f), b2Vec2(0.0f, 0.0f), true);

					bullets.push_back(bullet); // So we can keep track and kill them if we need
					BASS_ChannelPlay(ch2 = sndMgr.playSound("assets/sounds/player_shoot_sfx.wav"), FALSE);
				}

				if (inptMgr.getKey(absorb).getKeyHeld()) {
					//Show suction animation
					suction.setPos({ player.getPos().x,player.getPos().y, 0.25f });
					if (suctionCooldown < 0) {
						suctionCooldown = suctionCooldownMax;
						if (suctionFrame == 1) {
							suctionFrame = 2;
							suction.setGlyphset(suction2);

						}else if(suctionFrame == 2) {
							suctionFrame = 3;
							suction.setGlyphset(suction3);
						}
						else if(suctionFrame == 3) {
							suctionFrame = 1;
							suction.setGlyphset(suction1);
						}
					}
					else {
						suctionCooldown--;
					}
					for (int k = 0; k < redCells.size(); k++) {
						//If the cell is colliding with the player
						if (redCells.at(k)->isCollidingWith == &player) {
							absorbCooldown--;
							if (absorbCooldown < 0) {
								absorbCooldown = absorbCooldownMax;
								absorbtions++;
								//If we have a collision with the cell and player
								if (redCells.at(k)->getState() == 0) {
									redCells.at(k)->setState(1);
									redCells.at(k)->setGlyphset(rC2);
									BASS_ChannelPlay(ch2 = sndMgr.playSound("assets/sounds/redcell_absorb_sfx.wav"), FALSE);
								}
								else if (redCells.at(k)->getState() == 1) {
									redCells.at(k)->setState(2);
									redCells.at(k)->setGlyphset(rC3);
									BASS_ChannelPlay(ch2 = sndMgr.playSound("assets/sounds/redcell_absorb_sfx.wav"), FALSE);
								}
								else if (redCells.at(k)->getState() == 2) {
									redCells.at(k)->setState(3);
									redCells.at(k)->setGlyphset({ Glyph(' ', b2Vec2(0,0)) }); // Because its dead
									physMgr.getRigidBodyPtr(redCells.at(k))->SetTransform(b2Vec2(-20.0f, -20.0f), 0);
									redCells.at(k)->markForDeletion = true;
									BASS_ChannelPlay(ch2 = sndMgr.playSound("assets/sounds/redcell_absorb_finish_sfx.wav"), FALSE);
								}
							}
						}
					}
				}
				if (!inptMgr.getKey(absorb).getKeyHeld()) {
					suction.setPos({ -20.0,-20.0, 0.25f }); // Move suction back off screen
					absorbCooldown = absorbCooldownMax;
				}
			}

			/* Handle spawning of red blood cells*/
			{
				if (loops % 100 == 0) {
					RedCell* cell = new RedCell;
					cell->setGlyphset(rC1);
					cell->setPos({ rndrMgr.getWindowX() + 10.0f, 5 + rand() % ((int)rndrMgr.getWindowY() - 10) + 0.0f, 0.5f });
					physMgr.addRigidBody(cell, b2_staticBody, 2, 2);
					physMgr.getRigidBodyPtr(cell)->GetFixtureList()[0].SetSensor(true);
					redCells.push_back(cell);
				}

				for (int i = 0; i < redCells.size(); i++) {
					if ((redCells.at(i)->getState() < 3)) {

						physMgr.getRigidBodyPtr(redCells.at(i))->SetTransform(physMgr.getRigidBodyPtr(redCells.at(i))->GetTransform().p + b2Vec2(-0.001f * (rand() % 5), 0), 0);
						entitiesThisFrame.push_back(redCells.at(i));
					}
					else {//Put off screen
						physMgr.getRigidBodyPtr(redCells.at(i))->SetTransform(b2Vec2(-20.0f, -20.0f), 0);
						redCells.at(i)->markForDeletion = true;
					}
				}
			}

			/* Handle spawning of white blood cells */
			{
				if (loops % 400 == 0) {
					WhiteCell* cell = new WhiteCell;
					cell->setGlyphset(wC1);
					cell->setPos({ rndrMgr.getWindowX() + 10.0f, 5 + rand() % ((int)rndrMgr.getWindowY() - 10) + 0.0f, 0.5f });
					physMgr.addRigidBody(cell, b2_staticBody, 2.5, 2.5);
					physMgr.getRigidBodyPtr(cell)->GetFixtureList()[0].SetFilterData(enemyFilter);
					whiteCells.push_back(cell);
					cell->setCooldownMax(60 + (rand() % 60));
				}

				for (int i = 0; i < whiteCells.size(); i++) {
					if (!(whiteCells.at(i)->markForDeletion)) {
						physMgr.getRigidBodyPtr(whiteCells.at(i))->SetTransform(physMgr.getRigidBodyPtr(whiteCells.at(i))->GetTransform().p + b2Vec2(-0.001f * (rand() % 5) - 0.0005*(whiteCells.at(i)->getState()*2), 0), 0);
						entitiesThisFrame.push_back(whiteCells.at(i));
					
						if (whiteCells.at(i)->getCooldown() < 0 && whiteCells.at(i)->getPos().x > 0) {
							whiteCells.at(i)->setCooldown(whiteCells.at(i)->getCooldownMax());
							Bullet* bullet = new Bullet;
							bullet->setGlyphset(enemyBulletGS);
							bullet->setPos(whiteCells.at(i)->getPos());
							physMgr.addRigidBody(bullet, b2_dynamicBody, 1.2, 0.3);
							physMgr.getRigidBodyPtr(bullet)->GetFixtureList()[0].SetFilterData(enemyBulletFilter);
							physMgr.getRigidBodyPtr(bullet)->SetTransform({ whiteCells.at(i)->getPos().x / 100, whiteCells.at(i)->getPos().y / 100 }, 0);
							physMgr.getRigidBodyPtr(bullet)->ApplyLinearImpulse(b2Vec2(-0.00003f, 0.0f), b2Vec2(0.0f, 0.0f), true);

							enemyBullets.push_back(bullet); // So we can keep track and kill them if we need
							entities.push_back(bullet);
							BASS_ChannelPlay(ch2 = sndMgr.playSound("assets/sounds/whitecell_shoot_sfx.wav"), FALSE);
							
						}
						else {
							whiteCells.at(i)->setCooldown(whiteCells.at(i)->getCooldown() - 1);
						}
					}
					else {//Put off screen
						physMgr.getRigidBodyPtr(whiteCells.at(i))->SetTransform(b2Vec2(-20.0f, -20.0f), 0);
					}
				}
			}

			// Add bullets to entity list AND check if any are hitting a white blood cell
			for (int i = 0; i < bullets.size(); i++) {
				if (bullets.at(i)->getPos().x > 0 && bullets.at(i)->getPos().x < rndrMgr.getWindowX()) {

					for (int k = 0; k < whiteCells.size(); k++) {
						//If the cell is colliding with the player
						if (whiteCells.at(k)->isCollidingWith == bullets.at(i)) {
							bullets.at(i)->markForDeletion = true; //Kill the bullet

							physMgr.getRigidBodyPtr(bullets.at(i))->SetTransform(b2Vec2(-20.0, -20.0), 0);
							BASS_ChannelPlay(ch2 = sndMgr.playSound("assets/sounds/whitecell_hit_sfx.wav"), FALSE);
							if (whiteCells.at(k)->getState() == 0) {
								whiteCells.at(k)->setState(1);
								whiteCells.at(k)->setGlyphset(wC2);
							}
							else if (whiteCells.at(k)->getState() == 1) {
								whiteCells.at(k)->setState(2);
								whiteCells.at(k)->setGlyphset(wC3);
							}
							else if (whiteCells.at(k)->getState() == 2) {
								whiteCells.at(k)->setState(3);
								whiteCells.at(k)->setGlyphset(wC4);
							}
							else if (whiteCells.at(k)->getState() == 3) {
								whiteCells.at(k)->setGlyphset({ Glyphset(' ', b2Vec2(0,0)) });
								absorbtions += 6;
								//physMgr.getWorld()->DestroyBody(physMgr.getRigidBodyPtr(whiteCells.at(k)));
								whiteCells.at(k)->setPos(b2Vec3(-20.0f, -20.0f, 0));
								whiteCells.at(k)->markForDeletion = true;
							}
						}
					}
					entitiesThisFrame.push_back(bullets.at(i));
				}
			}

			/* Check if enemy bullets are touching player */
			{
				for (int i = 0; i < enemyBullets.size(); i++) {
					if (enemyBullets.at(i)->isCollidingWith == &player) {
						physMgr.getRigidBodyPtr(enemyBullets.at(i))->SetTransform(b2Vec2(-20.0f, -20.0f), 0);
						absorbtions -= 3;
					}
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

			/* Update Health Bar */
			healthBarGS.clear();
			for (int i = 0; i < rndrMgr.getWindowX(); i++) {
				if ((i*totalAbsorptions) / rndrMgr.getWindowX() < absorbtions) {
					//fill in
					healthBarGS.push_back(Glyph('>', { (float)(i - (rndrMgr.getWindowX() / 2)), 0 }));
				}
				else {
					healthBarGS.push_back(Glyph('_', { (float)(i - (rndrMgr.getWindowX() / 2)), 0 }));
				}
			}
			healthBar->setGlyphset(healthBarGS);
			framesPerHealth--;
			if (framesPerHealth < 1) {
				framesPerHealth = framesPerHealthMax;
				if ((player.getPos().x < -2.0f) || (player.getPos().x > rndrMgr.getWindowX() + 2.0f)) {
					absorbtions--;
				}
				absorbtions-= 0.05;
			}
			//entitiesThisFrame.reserve(entities.size() + particles.size() + );
			entitiesThisFrame.insert(entitiesThisFrame.end(), entities.begin(), entities.end());
			// This has to be done every frame because the pointers could become invalid.
			for (unsigned int i = 0; i < particles.size(); i++) {
				entitiesThisFrame.push_back(&(particles.at(i)));
			}


			if (absorbtions < 10) {
				HCHANNEL ch3 = sndMgr.playSound("assets/sounds/death_sfx.wav");
				BASS_ChannelSetAttribute(ch3, BASS_ATTRIB_VOL, (10 - absorbtions)/10);
				BASS_ChannelPlay(ch3, FALSE);
				rndrMgr.setWobble(true, 10 - absorbtions);
			}
			else {
				rndrMgr.setWobble(false, 0);
			}

			rndrMgr.RenderScene(entitiesThisFrame);

			for (int i = 0; i < entitiesThisFrame.size(); i++) {
				if (entitiesThisFrame.at(i)->markForDeletion) {
					pseudoDeleteBody(entitiesThisFrame.at(i), &physMgr);
				}
			}



			entitiesThisFrame.clear();
		}
	}
	if (absorbtions > 50) {
		winState = 1;
	}
	else {
		winState = 0;
	}
	std::chrono::milliseconds gameTimeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - beginGame;
	
	//Add a small wait so they dont skip past the end screen
	std::this_thread::sleep_for((std::chrono::milliseconds)2000);

	BASS_ChannelStop(bgmChannel);
	gameTime = (int)(gameTimeMillis.count() / 1000);
	std::pair<int, int> ret = { winState, gameTime };
	return ret;
}

