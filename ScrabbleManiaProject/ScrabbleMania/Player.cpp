/*
Luis Barajas
Eric Parton
*/

#include "Player.h"

Player::Player(int _id){
  id = _id;
  name = "";
}

Player::Player(int _id, string _name){
  id = _id;
  name = _name;
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
