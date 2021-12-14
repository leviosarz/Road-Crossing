#include "gameworld.h"

namespace LevelInfo {
    vector <SLevelInfo> levels;
    void LevelInfo::init() {
        {
            // level 1
            SLevelInfo level1;
            level1.id = 1;

            Object* ship1 = new Spawner("./asset/image/spaceship/spaceship3.png", "", 100, 80, 0.5f, 200, 350, false);
            level1.objs.push_back(ship1);
            Item* goal1 = new Item("./asset/image/goal/goal.gif", 40, 40, GOAL);
            goal1->place(rand()%300+50, 20);
            level1.items.push_back(goal1);
            // Insert new code as this
            // Items* abc = new ...
            // level1.items.push_back(abc)

            levels.push_back(level1);

            SLevelInfo level2;
            level2.id = 2;

            Object* ship2_0= new Spawner("./asset/image/spaceship/spaceship3.png", "", 100, 80, 0.5f, 200, 350, false);
            level2.objs.push_back(ship2_0);
            level2.items.push_back(goal1);
            Item* item2_f = new Item("./asset/image/frozen/frozen.png", 40, 40, FROZEN);
            item2_f->place(300, 100);
            level2.items.push_back(item2_f);
            Item* item2_i = new Item("./asset/image/invisible/invisible.png", 40, 40, INVISIBLE);
            item2_i->place(200, 150);
            level2.items.push_back(item2_i);
            levels.push_back(level2);
        }
    }
}

void GameWorld::initBackground(int id, string src) {
	backgroundTextures[id].loadFromFile(src);
	backgroundTexts[id].setTexture(backgroundTextures[id]);
	auto sz = backgroundTexts[id].getGlobalBounds();
	backgroundTexts[id].setScale(SCREEN_WIDTH / sz.width, SCREEN_HEIGHT / sz.height);
}

GameWorld::GameWorld() :
	window(sf::VideoMode(350, 700), "Road Crossing", sf::Style::Close),
	person("./asset/image/spaceShip0.png", 30, 30) 
{
    srand(time(0));

	// Enable vertical sync. (vsync)
	window.setVerticalSyncEnabled(true);
	// When a key is pressed, sf::Event::KeyPressed will be true only once
	window.setKeyRepeatEnabled(false);

	LevelInfo::init();

	backgroundTexts.resize(NUM_BACKGROUND);
	backgroundTextures.resize(NUM_BACKGROUND);

	initBackground(0, "./asset/image/background1.jpg");
	initBackground(1, "./asset/image/background2.png");
	initBackground(2, "./asset/image/background3.jpg");
	initBackground(3, "./asset/image/background4.jpg");

    isRunning = true;
}

GameWorld::~GameWorld() {
    for (auto level : LevelInfo::levels) {
        for (auto& obj : level.objs) delete obj;
        for (auto& item : level.items) delete item;
    }
}

void GameWorld::temporaryMessage(string message, float delaySecond, bool cleanScreen, float coorX, float coorY, int sz) {
    if (cleanScreen)
        window.clear(sf::Color(127, 127, 127));

    sf::Font font; font.loadFromFile("asset\\font\\ARCADECLASSIC.TTF");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(sz);
    text.setFillColor(sf::Color::White);
    text.setString(message);
    auto size = text.getGlobalBounds();
    text.setOrigin(size.width / 2, size.height / 2);
    text.setPosition(coorX, coorY);

    window.draw(text);
    window.display();

    sf::sleep(sf::seconds(delaySecond));

    return;
}

int GameWorld::menuAllInOne(Menu& menu, int idBG) {
    window.clear(sf::Color(127, 127, 127));

    while (true) {
        sf::Event event;
        while (window.pollEvent(event))
        {
             //Close the window if a key is pressed or if requested
            if (event.type == sf::Event::Closed) {
                isRunning = false;
                window.close();
            }

             //If a key is released
            if (event.type == sf::Event::KeyReleased)
            {
                switch (event.key.code)
                {
                     //Process the up, down, left and right keys
                case sf::Keyboard::Escape: return -1;
                case sf::Keyboard::Up: menu.moveUp(); break;
                case sf::Keyboard::Down: menu.moveDown(); break;
                case sf::Keyboard::Return: return menu.select();
                    
                default: break;
                }
            }
        }
        if (idBG != -1) window.draw(backgroundTexts[idBG]);
        menu.draw(window);
        window.display();
    }

    throw " oops ??";
    return -1;
}


void GameWorld::welcome() {
	int idBG = rand() % NUM_BACKGROUND;

    cout << "menu " << idBG << '\n';

	Menu menu(3, "asset\\font\\ARCADECLASSIC.TTF");

	menu.add("New Game");
	menu.add("Load Game");
	menu.add("Exit");

    while (window.isOpen()) {
        //window.draw(backgroundTexts[idBG]);
        switch (menuAllInOne(menu, idBG)) {
        case 0:
            std::cout << "go to new game\n";
            runLevel(1);
            break;
        case 1: {
            cout << " load game \n";
            Menu data(5, "asset\\font\\CONSOLAB.TTF");

            auto zeroPadding = [&](string& s, int sz = 2) {
                while (s.size() < sz) s.insert(s.begin(), '0');
                return s;
            };

            map <int, int> m;
            for (int i = 0; i < 5; i++) {
                ifstream inp("./log/" + to_string(i) + ".txt");
                if (!inp) {
                    cout << "file " << i << " is not existed\n";
                    data.add(to_string(i + 1) + ". ==============");
                }
                else {
                    int LV; inp >> LV; 
                    m[i] = LV;
                    LV++;
                    string lv = to_string(LV);
                    string day, month, year; inp >> day >> month >> year;
                    data.add(to_string(i + 1) + ". L" + zeroPadding(lv) + " " + zeroPadding(day) + "-" + zeroPadding(month) + "-" + zeroPadding(year, 4));
                }
                inp.close();
            }
            
            int t = menuAllInOne(data, idBG);
            cout << t << '\n';
            if (t == -1) break;
            if (m.count(t)) {
                runLevel(m[t]);
            }
            else runLevel(0);         
            break;
        }
        case 2:
            isRunning = false;
            break;
        }
        
        if (!isRunning) break;
    }
}

void GameWorld::runLevel(int idLevel) {
    sf::Font font; font.loadFromFile("asset\\font\\CONSOLAB.TTF");
    sf::Text levelLogo;
    levelLogo.setFont(font);
    levelLogo.setString("LEVEL " + to_string(idLevel + 1));
    levelLogo.setPosition(10, 670);
    levelLogo.setCharacterSize(15);

    int idBG = rand() % NUM_BACKGROUND;
    cout << idBG << '\n';

    objects = LevelInfo::levels[idLevel].objs;
	items = LevelInfo::levels[idLevel].items;

    person.Reset();

    // Flags for key pressed
    bool upFlag = false;
    bool downFlag = false;
    bool leftFlag = false;
    bool rightFlag = false;
    int test = 0;
    // Process events
    while (true) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close the window if a key is pressed or if requested
            if (event.type == sf::Event::Closed) {
                window.close();
                isRunning = false;
                return;
            }

            // If a key is pressed
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // If P is pressed, pause game
                case sf::Keyboard::P: {
                    Menu pauseScr(3, "asset\\font\\ARCADECLASSIC.TTF");
                    
                    pauseScr.add("Continue");
                    pauseScr.add("Save");
                    pauseScr.add("Quit");

                    switch (menuAllInOne(pauseScr, idBG)) {
                    case 0:
                        std::cout << "continue\n";
                        break;
                    case 1: {
                        // save game
                        Menu data(5, "asset\\font\\CONSOLAB.TTF");

                        auto zeroPadding = [&](string& s, int sz = 2) {
                            while (s.size() < sz) s.insert(s.begin(), '0');
                            return s;
                        };

                        map <int, int> m;
                        for (int i = 0; i < 5; i++) {
                            ifstream inp("./log/" + to_string(i) + ".txt");
                            if (!inp) {
                                cout << "file " << i << " is not existed\n";
                                data.add(to_string(i + 1) + ". ==============");
                            }
                            else {
                                int LV; inp >> LV;
                                m[i] = LV;
                                LV++;
                                string lv = to_string(LV);
                                string day, month, year; inp >> day >> month >> year;
                                data.add(to_string(i + 1) + ". L" + zeroPadding(lv) + " " + zeroPadding(day) + "-" + zeroPadding(month) + "-" + zeroPadding(year, 4));
                            }
                            inp.close();
                        }

                        int t = menuAllInOne(data, idBG);
                        if (t == -1) break;

                        ofstream out("./log/" + to_string(t) + ".txt");

                        time_t timeObj = time(nullptr);
                        tm aTime;
                        localtime_s(&aTime, &timeObj);

                        out << idLevel << ' ' << aTime.tm_mday << ' ' << 1 + aTime.tm_mon << ' ' << 1900 + aTime.tm_year << '\n';
                        cout << "save game\n";
                        break;
                    }
                    case 2:
                        return;
                    }
                    break;
                }
                case sf::Keyboard::X: {
                    //cout << "press X";
                    for (int i = 0; i < person.listItem.size(); i++) {
                        if (person.listItem[i].isFrozen()) {
                            for (auto& obj : objects) {
                                obj->stop();
                            }
                            person.listItem.erase(person.listItem.begin() + i, person.listItem.begin() + i + 1);
                        }
                    }
                    break;
                }
                case sf::Keyboard::Z:
                {
                    for (int i = 0; i < person.listItem.size(); i++) {
                        if (person.listItem[i].isInvisible()) {
                            person.Transparent();
                            person.listItem.erase(person.listItem.begin() + i, person.listItem.begin() + i + 1);
                        }
                    }
                    break;
                }
                    // Process the up, down, left and right keys
                case sf::Keyboard::Up:     upFlag = true; break;
                case sf::Keyboard::Down:    downFlag = true; break;
                case sf::Keyboard::Left:    leftFlag = true; break;
                case sf::Keyboard::Right:   rightFlag = true; break;
                default: break;
                }
            }

            // If a key is released
            if (event.type == sf::Event::KeyReleased)
            {
                switch (event.key.code)
                {
                    // Process the up, down, left and right keys
                case sf::Keyboard::Up:     upFlag = false; break;
                case sf::Keyboard::Down:    downFlag = false; break;
                case sf::Keyboard::Left:    leftFlag = false; break;
                case sf::Keyboard::Right:   rightFlag = false; break;
                default: break;
                }
            }

        //if (!person.listItem.empty()) {

        //    if (event.type == sf::Event::KeyPressed)
        //    {
        //        switch (event.key.code)
        //        {
        //            // If P is pressed, pause game
        //        
        //        default:break;
        //        }

        //    }
        //}
        }

        if (leftFlag) {
            cout << "left\n"; 
            person.SetDirection(DirectionPlayer::Left);
            person.Move();
        } 
        if (rightFlag) {
            cout << "right\n";  
            person.SetDirection(DirectionPlayer::Right);
            person.Move();
        }
        if (upFlag) {
            cout << "up\n";  
            person.SetDirection(DirectionPlayer::Up);
            person.Move();
        }
        if (downFlag) {
            cout << "down\n";
            person.SetDirection(DirectionPlayer::Down);
            person.Move();
        }

        // EXPLAIN THE ERRORS
        // Move inside the if, when it receive the button event, move directly
        // The problem of the previous is when up is pressing, and we press left, the if of Up is lower so the left will first set
        // as Direction Left but then go to up when be set as Up, results all UP
        // If we move Left lower than Up, the result will be all LEFT
         
        // Clear the window and apply grey background
        window.clear(sf::Color(127, 127, 127));

/*
    If reach GOAL, check idLevel vs NUM_LEVEL (in constant.h)
*/

        // Move Object
        for (auto& obj : objects) obj->move();

        // Draw Background, Player and Objects
        window.draw(backgroundTexts[idBG]);
        window.draw(levelLogo);
        for (auto& obj : objects) obj->render(window);
        person.Render(window);
        
        for (auto& item : items) {
            item->render(window);
        }
        // reach goal
        // first element is goal, second element is item
        if (person.isImpact(items[0])) {
            break;
        }
        // catch item
        for (int i = 1; i < items.size(); i++) {
            if (person.isImpact(items[i])) {
                person.addItem(*items[i]);              
                items.erase(items.begin() + i, items.begin() + i + 1);
            }
        }
        // use item
       


        // Collide with the objects
        for (auto& obj : objects) if (person.isImpact(obj)) {
            cout << "Game over\n";
            temporaryMessage("GAME OVER");
            return;
        }
        
        // Rotate and draw the sprite
        window.display();
    }
}