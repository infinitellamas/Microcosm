#pragma once
#include "Glyph.h"
#include <string>
#include <typeinfo>
#include <iostream>

Glyphset pMove1 = { \
// Middle section
Glyph('/',{ 1,1 }), Glyph('-',{ 0,1 }), Glyph('\\',{ -1,1 }), \
Glyph('|',{ 1,0 }), Glyph('Ï',{ 0,0 }), Glyph('|',{ -1,0 }), \
Glyph('\\',{ 1,-1 }), Glyph('-',{ 0,-1 }), Glyph('/',{ -1,-1 }),\

//Legs
Glyph('/',{ 0,-2 }), Glyph('o',{ 1,-3 }),\
Glyph('\\',{ 2,0 }), Glyph('o',{ 3,1 }),\
Glyph('o',{ -3,-1 }), Glyph('\\',{ -2,0 }), \
Glyph('/',{ 0,2 }), Glyph('o',{ -1,3 }) };

Glyphset pMove2 = { \
// Middle section
Glyph('/',{ 1,1 }), Glyph('-',{ 0,1 }), Glyph('\\',{ -1,1 }), \
Glyph('|',{ 1,0 }), Glyph('Ï',{ 0,0 }), Glyph('|',{ -1,0 }), \
Glyph('\\',{ 1,-1 }), Glyph('-',{ 0,-1 }), Glyph('/',{ -1,-1 }),\

//Legs
Glyph('/',{ 1,-2 }), Glyph('o',{ 2,-3 }),\
Glyph('\\',{ 2,1 }), Glyph('o',{ 3,2 }),\
Glyph('o',{ -3,-2 }), Glyph('\\',{ -2,-1 }), \
Glyph('/',{ -1,2 }), Glyph('o',{ -2,3 }) };

Glyphset pMove3 = { \
// Middle section
Glyph('/',{ 1,1 }), Glyph('-',{ 0,1 }), Glyph('\\',{ -1,1 }), \
Glyph('|',{ 1,0 }), Glyph('Ï',{ 0,0 }), Glyph('|',{ -1,0 }), \
Glyph('\\',{ 1,-1 }), Glyph('-',{ 0,-1 }), Glyph('/',{ -1,-1 }),\

//Legs
Glyph('|',{ 2,2 }), Glyph('o',{ 2,3 }),\
Glyph('-',{ 2,-2 }), Glyph('o',{ 3,-2 }),\
Glyph('-',{ -2,2 }), Glyph('o',{ -3,2 }), \
Glyph('|',{-2,-2 }), Glyph('o',{ -2,-3 }) };

Glyphset pMove4 = { \
// Middle section
Glyph('/',{ 1,1 }), Glyph('-',{ 0,1 }), Glyph('\\',{ -1,1 }), \
Glyph('|',{ 1,0 }), Glyph('Ï',{ 0,0 }), Glyph('|',{ -1,0 }), \
Glyph('\\',{ 1,-1 }), Glyph('-',{ 0,-1 }), Glyph('/',{ -1,-1 }),\

//Legs
Glyph('|',{ 1,2 }), Glyph('o',{ 1,3 }),\
Glyph('-',{ 2,-1 }), Glyph('o',{ 3,-1 }),\
Glyph('-',{ -2,1 }), Glyph('o',{ -3,1 }), \
Glyph('|',{ -1,-2 }), Glyph('o',{ -1,-3 }) };


Glyphset rC1 = {
	
	// Outer guards
	Glyph('-',{0,-2}), Glyph('|',{ -2,0 }), Glyph('|',{ 2,0 }),Glyph('-',{ 0,2 }),

	//Inner Layer
	Glyph('o',{ -1,-1 }), Glyph('0',{ 0,-1 }), Glyph('o',{ 1,-1 }),
	Glyph('0',{ -1,0 }),Glyph('Ï',{ 0,0 }), Glyph('0',{ 1,0 }),
	Glyph('o',{ -1,1 }),Glyph('0',{ 0,1 }),Glyph('o',{ 1,1 })
};

Glyphset rC2 = {
	//Inner Layer
	Glyph('o',{ -1,-1 }), Glyph('0',{ 0,-1 }), Glyph('o',{ 1,-1 }),
	Glyph('0',{ -1,0 }),Glyph('Ï',{ 0,0 }), Glyph('0',{ 1,0 }),
	Glyph('o',{ -1,1 }),Glyph('0',{ 0,1 }),Glyph('o',{ 1,1 })
};

Glyphset rC3 = {
	//Inner Layer
	Glyph('-',{ 0,-1 }),Glyph('|',{ -1,0 }),Glyph('Ï',{ 0,0 }), Glyph('|',{ 1,0 }),Glyph('-',{ 0,1 })
};


Glyphset wC1 = {
	//Inner section
	Glyph('X',{ -1,-1 }), Glyph('X',{ 0,-1 }), Glyph('X',{ 1,-1 }),
	Glyph('X',{ -1,0 }),Glyph('Ï',{ 0,0 }), Glyph('X',{ 1,0 }),
	Glyph('X',{ -1,1 }),Glyph('X',{ 0,1 }),Glyph('X',{ 1,1 }),

	//Outer layer
	Glyph('/',{ -1,-2 }), Glyph('v',{ 0,-2 }), Glyph('\\',{ 1,-2 }),
	Glyph('/',{ -2,-1 }),Glyph('}',{ -2,0 }), Glyph('\\',{ -2,1 }),
	Glyph('\\',{ 2,-1 }),Glyph('{',{ 2,0 }), Glyph('/',{ 2,1 }),
	Glyph('\\',{ -1,2 }), Glyph('^',{ 0,2 }), Glyph('/',{ 1,2 })
};

Glyphset wC2 = {
	//Inner section
	Glyph('X',{ -1,-1 }), Glyph('X',{ 0,-1 }), Glyph('X',{ 1,-1 }),
	Glyph('X',{ -1,0 }),Glyph('Ï',{ 0,0 }), Glyph('X',{ 1,0 }),
	Glyph('X',{ -1,1 }),Glyph('X',{ 0,1 }),Glyph('X',{ 1,1 }),

	//Outer layer
	 Glyph('v',{ 0,-2 }), Glyph('}',{ -2,0 }), Glyph('{',{ 2,0 }), Glyph('^',{ 0,2 })
};
Glyphset wC3 = {
	//Inner section
	Glyph('x',{ -1,-1 }), Glyph('X',{ 0,-1 }), Glyph('x',{ 1,-1 }),
	Glyph('X',{ -1,0 }),Glyph('Ï',{ 0,0 }), Glyph('X',{ 1,0 }),
	Glyph('x',{ -1,1 }),Glyph('X',{ 0,1 }),Glyph('x',{ 1,1 })
};
Glyphset wC4 = {
	Glyph('-',{ 0,-1 }), Glyph('|',{ -1,0 }), Glyph('Ï',{ 0,0 }), Glyph('|',{ 1,0 }),Glyph('-',{ 0,1 })
};






Glyphset scrambleSet(Glyphset original) {
	Glyphset g;

	return g;
}

class RedCell : public Entity {
public:
	RedCell() {
	}
	int getState() {
		return _state;
	}
	void setState(int newState) {
		_state = newState;
	}
private:
	int _state = 0; //0 for not absorbed, 1 for partial absorp, 2 for almost done, 3 for absorbed
};

class WhiteCell : public Entity {
public:
	int getState() {
		return _state;
	}
	void setState(int newState) {
		_state = newState;
	}

	int getCooldown() {
		return _shootCooldown;
	}
	void setCooldown(int newCooldown) {
		_shootCooldown = newCooldown;
	}

	int getCooldownMax() {
		return _shootCooldownMax;
	}
	void setCooldownMax(int newCooldownMax) {
		_shootCooldownMax = newCooldownMax;
	}
private:
	int _state = 0; //0 for stage full, 1 for stage 1, 2 for stage 2, 3 for stage 3, 4 for dead.
	int _shootCooldown = 0;
	int _shootCooldownMax = 120;
};


class Bullet : public Entity {
public:
	void setIsShot(bool isShot) {
		_isShot = isShot;
	}
	bool getIsShot() {
		return _isShot;
	}
private:
	bool _isShot = true;
};

class Player : public Entity {

};


class ContactListener : public b2ContactListener
{

	void BeginContact(b2Contact* contact) override {
		Entity* ent1 = ((Entity *)contact->GetFixtureA()->GetBody()->GetUserData());
		Entity* ent2 = ((Entity *)contact->GetFixtureB()->GetBody()->GetUserData());

		ent2->isCollidingWith = ent1;
		ent1->isCollidingWith = ent2;
		
	}

	void EndContact(b2Contact* contact) override {
		Entity* ent1 = ((Entity *)contact->GetFixtureA()->GetBody()->GetUserData());
		Entity* ent2 = ((Entity *)contact->GetFixtureB()->GetBody()->GetUserData());

		ent2->isCollidingWith = nullptr;
		ent1->isCollidingWith = nullptr;
	}

};





Glyphset createRandomLine(int length) {
	std::vector<char> choices = { '*','\\','/','-','_','#' };
	Glyphset g;
	for (int i = 0; i < length; i++) {
		char thisChar = choices.at(rand() % choices.size()); // Choose a random char to place
		g.push_back(Glyph(thisChar, b2Vec2((-length / 2) + i, rand()%2 - 1)));
	}
	return g;
}


int curPlayerAnim = 1;
void movePlayerAnim(Entity* e, bool dir) {
	if (curPlayerAnim == 1) {
		if (dir == true) {
			curPlayerAnim = 2;
			e->setGlyphset(pMove2);
		}
		else {
			curPlayerAnim = 4;
			e->setGlyphset(pMove4);
		}
	}
	else if (curPlayerAnim == 2) {
		if (dir == true) {
			curPlayerAnim = 3;
			e->setGlyphset(pMove3);
		}
		else {
			curPlayerAnim = 1;
			e->setGlyphset(pMove1);
		}
	}
	else if (curPlayerAnim == 3) {
		if (dir == true) {
			curPlayerAnim = 4;
			e->setGlyphset(pMove4);
		}
		else {
			curPlayerAnim = 2;
			e->setGlyphset(pMove2);
		}
	}
	else if (curPlayerAnim == 4) {
		if (dir == true) {
			curPlayerAnim = 1;
			e->setGlyphset(pMove1);
		}
		else {
			curPlayerAnim = 3;
			e->setGlyphset(pMove3);
		}
	}

}