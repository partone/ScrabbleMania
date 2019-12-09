/*
Luis Barajas
Eric Parton
*/

#include "Player.h"

Player::Player(int _id){
  id = _id;
  name = "";
  score = 0;
	playerType = ACTIVE;
  stillInGame = true;
}

Player::Player(int _id, string _name, player_type_t _playerType){
  id = _id;
  name = _name;
  score = 0;
	playerType = _playerType;
}
void Player::setName(string _name){
  name = _name;
}
string Player::getName(){
  return name;
}

void Player::addTileToHand(letterTile_t tile){
  hand.push_back(tile);
  printHand();
  return;
}

//Remove a single tile from the player based on its letter
void Player::removeTileFromHand(char letter) {
  for(int i = 0; (unsigned)i < hand.size(); i++) {
    if(hand[i].letter == letter) {
      hand.erase(hand.begin() + i);
      cout << "Removed " << letter << "from hand" << endl;
      printHand();
      return;
    }
  }
  cout << "Error, letter not found" << endl;
  return;
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

void Player::setPlayerType(player_type_t _playerType){
	playerType = _playerType;
}

player_type_t Player::getPlayerType(){
	return playerType;
}

int Player::getId() {
	return id;
}

//Prints a set of scrabble tiles
void Player::printHand() {
	for (int i = 0; (unsigned)i < hand.size(); i++) {
		cout << hand.at(i).letter << " " << hand.at(i).value;
		if ((unsigned)i != hand.size() - 1) {
			cout << " // ";
		}
	}
	cout << endl;
}

// Check that player has the tiles needed to form word
bool Player::hasNeededLetters(vector<char> neededLetters){
	int i = 0;
	int j;
	vector <letterTile_t> handCopy = hand;

	//For each letter needed
	while((unsigned)i < neededLetters.size()){
		j = 0;
		//For each letter in the copy of the player's hand
		while ((unsigned)j < handCopy.size()) {
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

void Player::printScore() {
	cout << name << "\t" << score << endl;
}
