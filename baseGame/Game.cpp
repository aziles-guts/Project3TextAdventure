//Isabelle Hoff + Skyla Gyimesi
//Reload saved game
#include "Game.h"
#include "Monster.h"
#include "Player.h"
#include "Item.h"
#include "Shopkeep.h"
#include <vector>
#include <fstream>

Game::Game(){
  //haha emp
}

Game::Game(Player playerIn, string savedItemsFileName, string saveStorageFileName, bool game){
  //player.setEqualTo(playerIn);
  setPlayer(playerIn);
  shopkeep.loadItems(savedItemsFileName);
  shopkeep.loadStorage(saveStorageFileName);
  if(!game){
    introShop();
  }
  theGround();
}


void Game::setPlayer(Player p){
  player.setName(p.getName());
  player.setWeapon(p.getWeapon());
  player.setArmor(p.getArmor());
  player.setPotion(p.getPotion());
  player.setMaxHealth(p.getMaxHealth());
  player.setHealth(p.getHealth());
  player.setStrength(p.getStrength());
  player.setDefense(p.getDefense());
  player.setGold(p.getGold());
  player.setHighestFloor(p.getHighestFloor());
  player.setNumPotion(p.getNumPotions());
}

void Game::introShop(){
  cout << "\"Hello I'm going to give you starting items now.\"\n";
  cout << "\"Here's a STICC.\"\n";
  player.setWeapon(shopkeep.getWeapons(0));
  cout << "\tGained " << player.getWeapon().getItemName() << ".\n";
  cout << "\"Here's some BUBBLEWRAP armor.\"\n";
  player.setArmor(shopkeep.getArmors(0));
  cout << "\tGained " << player.getArmor().getItemName() << "\n";
  cout << "\"Okay goodbye\"\n";
}

void Game::gameOver(){
  //ends the game to title returns -1
}

void Game::combat(int monsterId){
  bool escape = false;
  int menuInput;
  int attackRoll;
  bool playerTurn = true;
  Monster monster(monsterId);
  cout << "Infront of you is a " << monster.returnNameMonster() << " ready to fight." << endl;
  while(player.getHealth() > 0 && monster.returnHitPoints() > 0){
    while(playerTurn == true){
      cout << "What will you do?" << endl;
      cout << "HP: " << player.getHealth() << endl;
      cout << "1. Attack" << endl;
      cout << "2. Use Item" << endl;
      cin >> menuInput;
      switch(menuInput)
      {
      case 1:
        attackRoll = ((rand() % 20) + 1) + player.getAttackBonus();
        cout << endl;
        cout << "You swing with a " << player.getWeapon().getItemName() << " for " << attackRoll << "." << endl;
        if(attackRoll >= monster.returnDefense()){
          cout << "You hit!!!" << endl;
          cout << "You hit the " << monster.returnNameMonster() << " for " << player.getDamageMod() << endl;
          monster.takeDamage(player.getDamageMod());
        }else{
          cout << "You missed the " << monster.returnNameMonster() << endl;
        }
        playerTurn = false;
        break;
      case 2:
        if(player.getArmor().doesPlayerHave()){
          
          if(player.getNumPotions() > 0){player.usePotion(); cout << "You used a potion and now have " << player.getHealth() << " HP." << endl;}
          else{ cout << "You do not have any potions!\n"; }
          playerTurn = false;
        }else{
          cout << "You don't have any potions." << endl;
        }
        break;

      default:
        cout << "Invalid Input!" << endl;
        cout << string(2,'\n');
        break;
      }
    }
    
    cout << "The " << monster.returnNameMonster() << " attacks!" << endl;
    attackRoll = ((rand() % 20) + 1) + monster.returnStrength();
    if(attackRoll >= player.getDefense()){
      cout << "The " << monster.returnNameMonster() << " hits you for " << monster.returnDamageBase() << " hit points!" << endl;
      player.removeHealth(monster.returnDamageBase());
    }else{
      cout << "The " << monster.returnNameMonster() << " missed!" << endl;
    }
    playerTurn = true;
  }
  if(player.getHealth() < 1){
    cout << "Darkness surrounds you... You have died." << endl;
    death();
  }
  if(monster.returnHitPoints() < 1){
    cout << "You defeated the monster!" << endl;
    cout << "It dropped " << monster.returnRewardBase() << " gold!" << endl;
    player.addGold(monster.returnRewardBase());
    postCombat();
  }
}

void Game::postCombat(){
  Potion potion;
  int option;
  bool escape = false;
  while(escape == false){
    cout << "You stand now in an empty room of the tower." << endl;
    cout << "You still have " << player.getHealth() << " out of " << player.getMaxHealth() << endl;
    cout << "Floor: " << currentfloor << endl;
    cout << "Inputs" << endl;
    cout << "\t1. Continue up the tower." << endl;
    cout << "\t2. Drink potion." << endl;
    cout << "\t3. View stats." << endl;
    cout << "\t4. Return to bottom." << endl; 
    cin >> option;
    if(option == 1){
        generateFloor();
        break;
    }
    else if(option == 2){
      // use potion
        if(player.getNumPotions() > 0){
          player.usePotion();
          cout << "Your health is now " << player.getHealth() << "!" << endl;
        }else{
          cout << "You do not have any potions.\n";
        }
    }
    else if(option == 3){
      player.printStats();
    }
    else if(option == 4){
      break;
    }
    else{
      cout << "Invalid input." << endl;
    }
  }
}

void Game::death(){
  player.setGold(player.getGold() * 0.6);
  theGround();
}

void Game::theGround(){
  int menuChoice;
  bool menuExit = false;
  currentfloor = 0;
  player.setMaxHealth(30 + player.getHighestFloor() * 2);
  player.setHealth(player.getMaxHealth()); 
  cout << string(2, '\n');
  cout << "You stand at the base of the tower, what would you like to do?" << endl;
  while(menuExit == false){
    cout << "Input 1, to go into the tower, input 2 to talk with the shopkeep, and 3 to save and quit." << endl;
    cin >> menuChoice;
    if(menuChoice == 1){ generateFloor(); }
    else if(menuChoice == 2){ 
      shopkeep.displayMainShopMenu(player);
    }
    else if(menuChoice == 3){
      //leave
      saveGame();
      break;
    }
    else{
      cout << "Invalid input" << endl;
    }
  }
}

void Game::generateFloor(){
  currentfloor++;
  int option;
  for(int i = -1; i < currentfloor; i++){ option = (rand() % 100) + 1; }
  if(option > 2){
    int id = chooseMonster();
    recordMonster(id);
    combat(id);
  }else{
    int coins = (rand() % 50) + 5;
    cout << "Somehow you managed to find a floor with no monsters, just some coins scattered across the floor." << endl;
    cout << "You get " << coins << " gold!" << endl;
    player.addGold(coins);
    postCombat();
  }
}

int Game::chooseMonster(){//returns a monster id to load for floor generation
  cout << "Finding ID CHOOSEMONSTER" << endl;
  int randNum;
  int tableNo;
  int lineNo;
  if(currentfloor > 15){
    randNum = (rand() % 10) + 1; //1 to 10
    if(randNum > 5){//table 3
      tableNo = 3;
      lineNo = (rand() % 10) + 1;
      return readTable(tableNo, lineNo);
    }
    if(randNum > 2){
      tableNo = 2;
      lineNo = (rand() % 10) + 1;
      return readTable(tableNo, lineNo);
    }
    if(randNum > 0){
      tableNo = 1;
      lineNo = (rand() % 15) + 1;
      return readTable(tableNo, lineNo);
    }
  }else if(currentfloor > 8){
    randNum = (rand() % 10) + 1; //1 to 10
    if(randNum > 8){//table 3
      tableNo = 3;
      lineNo = (rand() % 10) + 1;
      return readTable(tableNo, lineNo);
    }
    if(randNum > 4){
      tableNo = 2;
      lineNo = (rand() % 10) + 1;
      return readTable(tableNo, lineNo);
    }
    if(randNum > 0){
      tableNo = 1;
      lineNo = (rand() % 15) + 1;
      return readTable(tableNo, lineNo);
    }
  }else if(currentfloor >= 0){
    randNum = (rand() % 10) + 1; //1 to 10
    if(randNum > 9){//table 3
      tableNo = 3;
      lineNo = (rand() % 10) + 1;
      return readTable(tableNo, lineNo);
    }
    if(randNum > 7){
      tableNo = 2;
      lineNo = (rand() % 10) + 1;
      return readTable(tableNo, lineNo);
    }
    if(randNum > 0){
      tableNo = 1;
      lineNo = (rand() % 15) + 1;
      return readTable(tableNo, lineNo);
    }
  }
  return 0;
}

int Game::readTable(int tableNo, int lineNo){
  string tableNoStr = to_string(tableNo);
  string fileHead = "./res/monsters/monsterTable";
  ifstream currentTable;
  string line;
  fileHead.append(tableNoStr);
  fileHead.append(".dat");
  currentTable.open(fileHead);
  for(int i = 0; i < lineNo; i++){
    getline(currentTable, line);
  }
  currentTable.close();
  return stoi(line);
}

void Game::saveGame(){
  ofstream writeSave;
  cout << string(50, '\n');
  writeSave.open("save.dat");
  writeSave << player.getName() << endl; // name
  writeSave << player.getMaxHealth() << endl; // maxHP
  writeSave << player.getStrength() << endl;  // strength
  writeSave << player.getDefense() << endl;   // defense
  writeSave << player.getGold() << endl;      // gold/coins
  writeSave << player.getHighestFloor() << endl;   // highest floor reached
  writeSave << "storage.dat" << endl;         // saved storage/inventory file name
  writeSave << "savedItems.dat" << endl;      // saved items file name
  writeSave << 1 << endl;   // 0 if this is a new game, 1 if not
  writeSave << player.getNumPotions() << endl;   // number of potions in inventory
  writeSave.close();
  saveScore(highscoreCalculation());
  sortScore();
  gameOver();
}

int Game::highscoreCalculation(){
  int gold = player.getGold();
  int highestFloor = player.getHighestFloor();
  int score = (gold * 1.2) + (highestFloor * 12.7) + 1;
  return score;
}

void Game::recordMonster(int mId){
  string idStr = to_string(mId);
  string line;
  vector<string> listedIds;
  ifstream currentFileR;
  ofstream currentFileW;
  currentFileR.open("./save/monsterEncounters.txt");
  while(getline(currentFileR, line)){
    listedIds.push_back(line);
  }
  currentFileR.close();
  listedIds.push_back(idStr);
  currentFileW.open("./save/monsterEncounters.txt");
  for (int i = 0; i < listedIds.size(); i++){
    currentFileW << listedIds.at(i) << endl;
  } 
}

void Game::saveScore(int score){
  string scoreStr = to_string(score);
  string line;
  vector<string> scores;
  ifstream currentFile;
  ofstream currentFileW;
  currentFile.open("highscore.dat");
  while(getline(currentFile, line)){
    scores.push_back(line);
  }
  currentFile.close();
  scores.push_back(scoreStr);
  currentFileW.open("highscore.dat");
  for (int i = 0; i < scores.size(); i++){
    currentFileW << scores.at(i) << endl;
  }
}

void Game::sortScore(){
  // intake file info to process
  vector<int> scores = intakeFileToSort("highscore.dat");
  vector<int> scoresSorted;

  // sort the info:
  // while it's not sorted...
  while(true){
    // find the largest value and put it in scoresSorted
    int index = 0;
    for(int pos = 1; pos < scores.size() - 1; pos++){
      if(scores.at(index) < scores.at(pos)){
        index = pos;
      }
    }
    scoresSorted.push_back(scores.at(index));
    scores.erase(scores.begin()+index); // erases the indexth element
    // if scores is empty, leave the loop
    if(scores.size() == 0 ){ break; }
  }

  // output file info
  outputSortedFile(scoresSorted, "highscore.dat");
}

void Game::outputSortedFile(vector<int> list, string fileIn){
  ofstream myFile;
  myFile.open(fileIn);
  if(!myFile.is_open()) { cout << "File is not open.\n"; }
  string line;
  for(int score : list){
    myFile << score << endl;
  }
  myFile.close();
}

vector<int> Game::intakeFileToSort(string fileIn){
  vector<int> list;
  ifstream myFile;
  myFile.open(fileIn);
  if(!myFile.is_open()) { cout << "File is not open.\n"; }
  string line;
  while(getline(myFile, line)){
    list.push_back(stoi(line));
  }
  myFile.close();
  return list;
}
