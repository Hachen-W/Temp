#include <SFML/Graphics.hpp>
#include "Game.h"

using std::dynamic_pointer_cast;
using std::make_shared;
void entity_to_do_step(Game& game, int type_entity);
void drawwing(Game& game, vector<sf::Texture> textures, sf::RenderWindow& window);

int main()
{
    Game game;
    game.try_generate_game_field();
    Player player;
    player.landscape = game.landscape;
    
    //std::cout<< game.landscape.cells.matrix[9][0].ptr_entities.size() << std::endl;
   // game.to_do_step();
   // 
    //std::cout<< game.landscape.cells.matrix[9][0].ptr_entities.size() << std::endl;
    sf::RenderWindow window(sf::VideoMode(800,800), "SFML works!");
        
    sf::Sprite sprite;
        
    sf::Texture texture_land;
    texture_land.loadFromFile("images/land.jpg");
        
    sf::Texture texture_moun;
    texture_moun.loadFromFile("images/moun.jpg");
        
    sf::Texture texture_river;
    texture_river.loadFromFile("images/river.png");
       
    sf::Texture texture_castle;
    texture_castle.loadFromFile("images/castle.png");
        
    sf::Texture texture_lair;
    texture_lair.loadFromFile("images/lair.png");

    sf::Texture texture_tower;
    texture_tower.loadFromFile("images/tower.png");

    sf::Texture texture_wall;
    texture_wall.loadFromFile("images/wall.png");

    sf::Texture texture_light_infantry;
    texture_light_infantry.loadFromFile("images/light_infantry.png");

    sf::Texture texture_heavy_infantry;
    texture_heavy_infantry.loadFromFile("images/heavy_infantry.png");

    sf::Texture texture_avia_land;
    texture_avia_land.loadFromFile("images/avia_land.png");

    sf::Texture texture_avia_river;
    texture_avia_river.loadFromFile("images/avia_river.png");

    sf::Texture texture_avia_moun;
    texture_avia_moun.loadFromFile("images/avia_moun.png");

    vector<sf::Texture> textures = {texture_land, texture_moun, texture_river, texture_castle, texture_lair, texture_tower, texture_wall, texture_light_infantry, texture_heavy_infantry, texture_avia_land, texture_avia_moun, texture_avia_river};

    for (float i = 0; i < 10; i++){
        for (float j = 0; j < 10; j++){ 
            Cell cell = (game.landscape -> cells).matrix[i][j];
            switch (static_cast<int>(cell.type))
            {
                case 1:
                    sprite.setTexture(texture_land);
                    sprite.setScale(0.165, 0.165);
                    break;
                case 2:
                    sprite.setTexture(texture_moun);
                    sprite.setScale(0.165, 0.165);
                    break;
                case 3:
                    sprite.setTexture(texture_river);
                    sprite.setScale(0.165, 0.165);
                    break;
            }

            if (cell.ptr_entities.size() == 0){
                sprite.setPosition(i * 100, j * 100);
                window.draw(sprite);
                continue;
            }
            for (shared_ptr<Entity> entity : cell.ptr_entities){
                if (dynamic_pointer_cast<Construction>(entity) == nullptr){
                    continue;
                }else{
                    if (dynamic_pointer_cast<Castle>(entity) != nullptr){
                        sprite.setTexture(texture_castle);
                        sprite.setScale(0.165, 0.165);
                        cell.type = static_cast<Land_Type>(1);
                        player.castle_pos[0] = i;
                        player.castle_pos[1] = j;
                    }
                    if (dynamic_pointer_cast<Lair>(entity) != nullptr){
                        sprite.setTexture(texture_lair);
                        sprite.setScale(0.165, 0.165);
                        cell.type = static_cast<Land_Type>(1);
                    }
                    if (dynamic_pointer_cast<Tower>(entity) != nullptr){
                        sprite.setTexture(texture_tower);
                        sprite.setScale(0.165, 0.165);
                    }
                    if (dynamic_pointer_cast<Wall>(entity) != nullptr){
                        sprite.setTexture(texture_wall);
                        sprite.setScale(0.165, 0.165);
                    }
                }
            }  
            sprite.setPosition(i * 100, j * 100);
            window.draw(sprite);
        }
    }

    while (window.isOpen())
    {   
        bool status = 0;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
             if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Right){
                    std::cout << "Person to do step" << std::endl;
                    status = game.to_do_step();
                    drawwing(game, textures, window);
            //        std::cout << "Size is " << game.enemies.size() << std::endl;
                    
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                  //  std::cout << "Tower was created" << std::endl;
                    int pos_x = event.mouseButton.x / 100;
                    int pos_y = event.mouseButton.y / 100;
                
                    if ( ((game.landscape)-> cells)[pos_x][pos_y].ptr_entities.size() != 0){
                        for (auto entity : ((game.landscape)-> cells)[pos_x][pos_y].ptr_entities){
                            if (dynamic_pointer_cast<Tower>(entity) != nullptr){
                                player.upgrade_tower(pos_x, pos_y);
                            }
                            if (dynamic_pointer_cast<Castle>(entity) != nullptr){
                                player.upgrade_castle();
                            }
                        }
                    } else player.build_tower(pos_x, pos_y);
                 //   std::cout << (player.landscape -> cells).matrix[pos_x][pos_y].ptr_entities.size() << std::endl;
                 //   game.landscape = *(player.landscape);
                //    std::cout << "mouse x: " << event.mouseButton.x / 100 << std::endl;
                //    std::cout << "mouse y: " << event.mouseButton.y / 100 << std::endl;
                    drawwing(game, textures, window);
                   // std::cout << game.landscape.cells.matrix[0][8].ptr_entities.size() << std::endl;
                   // std::cout << (player.landscape -> cells).matrix[0][8].ptr_entities.size() << std::endl;

                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                  //  std::cout << "Tower was created" << std::endl;
                    int pos_x = event.mouseButton.x / 100;
                    int pos_y = event.mouseButton.y / 100;
                
                    if ( ((game.landscape)-> cells)[pos_x][pos_y].ptr_entities.size() != 0){
                        for (auto entity : ((game.landscape)-> cells)[pos_x][pos_y].ptr_entities){
                            if (dynamic_pointer_cast<Wall>(entity) != nullptr){
                                player.fix_wall(pos_x, pos_y);
                            }
                            if (dynamic_pointer_cast<Castle>(entity) != nullptr){
                                player.upgrade_castle();
                            }
                        }
                    } else player.build_wall(pos_x, pos_y);

                    drawwing(game, textures, window);

                }
            }
            
        }
        window.display();
        if (status == 1) break;
    }
    return 0;
}


void drawwing(Game& game, vector<sf::Texture> textures, sf::RenderWindow& window){
    sf::Sprite sprite;

    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){ 
            Cell cell = (game.landscape -> cells).matrix[i][j];
            if (cell.ptr_entities.size() == 0){
                
                switch(static_cast<int>(cell.type)){
                    case 1:
                        sprite.setTexture(textures[0]);
                        sprite.setScale(0.165, 0.165);
                        break;
                    case 2:
                        sprite.setTexture(textures[1]);
                        sprite.setScale(0.165, 0.165);
                        break;
                    case 3:
                        sprite.setTexture(textures[2]);
                        sprite.setScale(0.165, 0.165);
                        break;
                }
                sprite.setPosition(i * 100, j * 100);
                window.draw(sprite);

                continue;
            }
            for (shared_ptr<Entity> entity : cell.ptr_entities){
               // std::cout << i << j << "I need to draw somethink ..." << std::endl;
                if (dynamic_pointer_cast<Enemy>(entity) != nullptr){
                 //   std::cout << i << j << "I need to draw some enemy ..." << std::endl;
                  //  std::cout << "Drawwing enemy.." << std::endl;
                    if (dynamic_pointer_cast<Light_Infantry>(entity) != nullptr){
                  //      std::cout << "Drawwing enemy..1" << std::endl;
                        sprite.setTexture(textures[7]);
                        sprite.setScale(0.165, 0.165);
                    }
                    if (dynamic_pointer_cast<Heavy_Infantry>(entity) != nullptr){
                  //      std::cout << "Drawwing enemy..2" << std::endl;
                        sprite.setTexture(textures[8]);
                        sprite.setScale(0.165, 0.165);
                    }
                    if (dynamic_pointer_cast<Aviation>(entity) != nullptr){
                  //      std::cout << "Drawwing enemy..3" << std::endl;
                        switch(static_cast<int>(cell.type))
                        {
                            case 1:
                                sprite.setTexture(textures[9]);
                                break;
                            case 2:
                                sprite.setTexture(textures[10]);
                                break;
                            case 3:
                                sprite.setTexture(textures[11]);
                                break;
                        }
                        sprite.setScale(0.165, 0.165);
                    }
                }
                
                if (dynamic_pointer_cast<Construction>(entity) != nullptr){      
                  //  std::cout << i << j << "I need to draw some construction ..." << std::endl;           
                    if (dynamic_pointer_cast<Castle>(entity) != nullptr){
                        sprite.setTexture(textures[3]);
                        sprite.setScale(0.165, 0.165);
                        cell.type = static_cast<Land_Type>(1);
                    }
                    if (dynamic_pointer_cast<Lair>(entity) != nullptr){
                        sprite.setTexture(textures[4]);
                        sprite.setScale(0.165, 0.165);
                        cell.type = static_cast<Land_Type>(1);
                        break;
                    }
                    if (dynamic_pointer_cast<Tower>(entity) != nullptr){
                        sprite.setTexture(textures[5]);
                        sprite.setScale(0.165, 0.165);
                    }
                    if (dynamic_pointer_cast<Wall>(entity) != nullptr){
                        sprite.setTexture(textures[6]);
                        sprite.setScale(0.165, 0.165);
                    }
                }
                
                sprite.setPosition(i * 100, j * 100);
                window.draw(sprite);
            }  
            
        }
    }
    window.display();
}
/*


            Cell cell = cells[current_pos[0] + x][current_pos[1] + y];
            if (cell.ptr_entities.size() == 0){
                 continue;
            }else for(shared_ptr<Entity> entity : cell.ptr_entities){
                if (dynamic_pointer_cast<Enemy>(entity) != nullptr){
                    attack_enemy(dynamic_pointer_cast<Enemy>(entity));
                }
                */
