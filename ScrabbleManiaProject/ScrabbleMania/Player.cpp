/*
Luis Barajas
Eric Parton
*/

#include "Player.h"

Player::Player(int _id){
  id = _id;
  name = "";
  score = 0;
}

Player::Player(int _id, string _name){
  id = _id;
  name = _name;
  score = 0;
}
void Player::setName(string _name){
  name = _name;
}
string Player::getName(){
  return name;
}

void Player::addTileToHand(letterTile_t tile){
  hand.push_back(tile);
}

vector<letterTile_t> * Player::getHand(){
  return &hand;
}

void Player::setHand(vector<letterTile_t> newHand) {
	hand = newHand;
}

void Player::setScore(int _score) {
	score = _score;
}

int Player::getScore() {
	return score;
}

int Player::getId() {
	return id;
}

// Check that player has the tiles needed to form word
bool Player::hasNeededLetters(vector<char> neededLetters){
	int i = 0;
	int j;
	vector <letterTile_t> handCopy = hand;

	//For each letter needed
	while(i < neededLetters.size()){
		j = 0;
		//For each letter in the copy of the player's hand
		while (j < handCopy.size()) {
			//If the hand letter matches the letter needed, erase it in both vectors
			if (handCopy[j].letter == neededLetters[i]) {
				handCopy.erase(handCopy.begin() + j);
				neededLetters.erase(neededLetters.begin() + i);
				//Reduce the i counter to account for there being a new letter in its place
				i--;
				//Finish the search for this letter
				j = handCopy.size();
			}
			else {
				//Check the next index
				j++;
			}
		}
		//Check the next letter needed
		i++;
	}
	return neededLetters.empty();
}
