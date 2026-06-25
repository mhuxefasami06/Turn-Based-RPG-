#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Player
{
    char name[30];
    int hp, maxHp;
    int mp, maxMp;
    int attack;
    int level;
    int exp;
    int gold;
    int potions;
};

struct Enemy
{
    char name[30];
    int hp;
    int attack;
};

struct Player player;

/* Function Prototypes */
void createCharacter();
void showStats();
void gameMenu();
void battle();
void enemyTurn(struct Enemy enemy);
void useSkill(struct Enemy *enemy);
void usePotion();
void levelUp();
void shop();
void saveGame();
void loadGame();

int main()
{
    int choice;

    srand(time(NULL));

    printf("===== TEXT RPG GAME =====\n");
    printf("1. New Game\n");
    printf("2. Load Game\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if(choice == 2)
        loadGame();
    else
        createCharacter();

    gameMenu();

    return 0;
}

void createCharacter()
{
    printf("\nEnter your character name: ");
    scanf("%s", player.name);

    player.maxHp = 100;
    player.hp = 100;

    player.maxMp = 50;
    player.mp = 50;

    player.attack = 20;

    player.level = 1;
    player.exp = 0;
    player.gold = 50;

    player.potions = 3;

    printf("\nCharacter Created Successfully!\n");
}

void showStats()
{
    printf("\n===== PLAYER STATS =====\n");
    printf("Name: %s\n", player.name);
    printf("Level: %d\n", player.level);
    printf("EXP: %d\n", player.exp);
    printf("HP: %d/%d\n", player.hp, player.maxHp);
    printf("MP: %d/%d\n", player.mp, player.maxMp);
    printf("Attack: %d\n", player.attack);
    printf("Gold: %d\n", player.gold);
    printf("Potions: %d\n", player.potions);
}

void battle()
{
    struct Enemy enemy;

    strcpy(enemy.name, "Goblin");

    enemy.hp = 60 + rand() % 21;
    enemy.attack = 10 + rand() % 6;

    printf("\n===== BATTLE START =====\n");
    printf("A wild %s appeared!\n", enemy.name);

    while(enemy.hp > 0 && player.hp > 0)
    {
        int choice;
        int damage;

        printf("\n%s HP: %d/%d   MP: %d/%d\n",
               player.name,
               player.hp,
               player.maxHp,
               player.mp,
               player.maxMp);

        printf("%s HP: %d\n", enemy.name, enemy.hp);

        printf("\n1. Attack\n");
        printf("2. Skill (Fireball)\n");
        printf("3. Use Potion\n");

        printf("Choice: ");
        scanf("%d", &choice);

        if(choice == 1)
        {
            damage = player.attack;

            enemy.hp -= damage;

            printf("You dealt %d damage!\n", damage);
        }
        else if(choice == 2)
        {
            useSkill(&enemy);
        }
        else if(choice == 3)
        {
            usePotion();
        }
        else
        {
            printf("Invalid Choice!\n");
            continue;
        }

        if(enemy.hp <= 0)
            break;

        enemyTurn(enemy);
    }

    if(player.hp > 0)
    {
        printf("\nYou defeated the enemy!\n");

        player.exp += 20;
        player.gold += 30;

        printf("You gained 20 EXP and 30 Gold.\n");

        levelUp();
    }
    else
    {
        int restart;

        printf("\nGAME OVER!\n");
        printf("Restart Game? (1=Yes, 0=No): ");
        scanf("%d", &restart);

        if(restart == 1)
        {
            createCharacter();
        }
        else
        {
            exit(0);
        }
    }
}

void enemyTurn(struct Enemy enemy)
{
    int damage;

    printf("\nEnemy Turn!\n");

    /* Simple Enemy AI */
    if(enemy.hp < 20)
    {
        printf("Enemy becomes desperate and attacks fiercely!\n");
        damage = enemy.attack + 5;
    }
    else
    {
        damage = enemy.attack;
    }

    player.hp -= damage;

    printf("Enemy dealt %d damage!\n", damage);
}

void useSkill(struct Enemy *enemy)
{
    if(player.mp >= 15)
    {
        player.mp -= 15;

        enemy->hp -= 35;

        printf("You used Fireball!\n");
        printf("Fireball dealt 35 damage!\n");
    }
    else
    {
        printf("Not enough MP!\n");
    }
}

void usePotion()
{
    if(player.potions > 0)
    {
        player.potions--;

        player.hp += 30;

        if(player.hp > player.maxHp)
            player.hp = player.maxHp;

        printf("You used a Health Potion.\n");
        printf("Recovered 30 HP.\n");
    }
    else
    {
        printf("No potions left!\n");
    }
}

void levelUp()
{
    if(player.exp >= player.level * 50)
    {
        player.exp = 0;

        player.level++;

        player.maxHp += 20;
        player.maxMp += 10;
        player.attack += 5;

        player.hp = player.maxHp;
        player.mp = player.maxMp;

        printf("\n***** LEVEL UP! *****\n");
        printf("You are now Level %d\n", player.level);
    }
}

void shop()
{
    int choice;

    do
    {
        printf("\n===== SHOP =====\n");
        printf("Gold: %d\n", player.gold);

        printf("1. Buy Potion (20 Gold)\n");
        printf("0. Exit Shop\n");

        printf("Choice: ");
        scanf("%d", &choice);

        if(choice == 1)
        {
            if(player.gold >= 20)
            {
                player.gold -= 20;
                player.potions++;

                printf("Potion Purchased!\n");
            }
            else
            {
                printf("Not enough Gold!\n");
            }
        }

    } while(choice != 0);
}

void saveGame()
{
    FILE *file;

    file = fopen("save.txt", "w");

    if(file == NULL)
    {
        printf("Unable to save game.\n");
        return;
    }

    fprintf(file, "%s %d %d %d %d %d %d %d %d %d\n",
            player.name,
            player.hp,
            player.maxHp,
            player.mp,
            player.maxMp,
            player.attack,
            player.level,
            player.exp,
            player.gold,
            player.potions);

    fclose(file);

    printf("Game Saved Successfully!\n");
}

void loadGame()
{
    FILE *file;

    file = fopen("save.txt", "r");

    if(file == NULL)
    {
        printf("No saved game found.\n");

        createCharacter();
        return;
    }

    fscanf(file, "%s %d %d %d %d %d %d %d %d %d",
           player.name,
           &player.hp,
           &player.maxHp,
           &player.mp,
           &player.maxMp,
           &player.attack,
           &player.level,
           &player.exp,
           &player.gold,
           &player.potions);

    fclose(file);

    printf("Game Loaded Successfully!\n");
}

void gameMenu()
{
    int choice;

    do
    {
        printf("\n===== GAME MENU =====\n");
        printf("1. Explore (Battle)\n");
        printf("2. Shop\n");
        printf("3. Show Stats\n");
        printf("4. Save Game\n");
        printf("5. Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                battle();
                break;

            case 2:
                shop();
                break;

            case 3:
                showStats();
                break;

            case 4:
                saveGame();
                break;

            case 5:
                printf("Thanks for playing!\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while(choice != 5);
}