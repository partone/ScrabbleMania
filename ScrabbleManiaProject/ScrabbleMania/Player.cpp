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

// FIXME: Check that player has the tiles needed to form word
bool Player::hasNeededLetters(vector<char> neededLetters){
  int i = 0;
  set<int> usedIndexes;
  while(i < hand.size()){
    for(int j = 0; j < neededLetters.size(); j++){
      if((neededLetters[j] == hand[i].letter) && (usedIndexes.find(i) != usedIndexes.end())){
        cout << neededLetters[j] << endl;
        usedIndexes.insert(i);
        neededLetters.erase(neededLetters.begin() + j);
        break;
      }
    }
    i++;
  }

  return neededLetters.empty();
}
