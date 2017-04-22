#pragma once
#include "Glyph.h"

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