#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Player{
    char name[30];
    int hp,maxHp;
    int mp,maxMp;
    int attack;
    int level;
    int exp;
    int gold;
    int potions;
};

struct Enemy{
    char name[30];
    int hp;
    int attack;
};

struct Player player;
struct Enemy enemy;

void createCharacter();
void showStats();
void battle();
void shop();
void saveGame();
void loadGame();
void gameMenu();
void levelUp();

int main(){
    int choice;
    srand(time(NULL));
    printf("===== TEXT RPG GAME =====\n1. New Game\n2. Load Game\nChoice: ");
    scanf("%d",&choice);

    if(choice==2) loadGame();
    else createCharacter();

    gameMenu();
    return 0;
}

void createCharacter(){
    printf("Enter Name: ");
    scanf("%29s",player.name);
    player.maxHp=100; player.hp=100;
    player.maxMp=50; player.mp=50;
    player.attack=20;
    player.level=1; player.exp=0;
    player.gold=50; player.potions=3;
}

void showStats(){
    printf("\nName: %s\nLevel: %d\nHP: %d/%d\nMP: %d/%d\nAttack: %d\nEXP: %d\nGold: %d\nPotions: %d\n",
    player.name,player.level,player.hp,player.maxHp,player.mp,player.maxMp,player.attack,player.exp,player.gold,player.potions);
}

void battle(){
    strcpy(enemy.name,"Goblin");
    enemy.hp=70;
    enemy.attack=12;
    while(enemy.hp>0 && player.hp>0){
        int c;
        printf("\nYour HP:%d  Enemy HP:%d\n",player.hp,enemy.hp);
        printf("1.Attack\n2.Fireball\n3.Potion\nChoice: ");
        scanf("%d",&c);

        if(c==1){
            enemy.hp-=player.attack;
            printf("You attacked!\n");
        }else if(c==2){
            if(player.mp>=15){
                player.mp-=15;
                enemy.hp-=35;
                printf("Fireball used!\n");
            }else printf("Not enough MP!\n");
        }else if(c==3){
            if(player.potions>0){
                player.potions--;
                player.hp+=30;
                if(player.hp>player.maxHp) player.hp=player.maxHp;
            }else printf("No potions!\n");
        }else continue;

        if(enemy.hp>0){
            player.hp-=enemy.attack;
            printf("Enemy attacked!\n");
        }
    }

    if(player.hp>0){
        printf("You Win!\n");
        player.exp+=20;
        player.gold+=30;
        levelUp();
    }else{
        printf("Game Over!\n");
        createCharacter();
    }
}

void levelUp(){
    if(player.exp>=50){
        player.level++;
        player.exp=0;
        player.maxHp+=20;
        player.maxMp+=10;
        player.attack+=5;
        player.hp=player.maxHp;
        player.mp=player.maxMp;
        printf("Level Up!\n");
    }
}

void shop(){
    int c;
    do{
        printf("\nGold:%d\n1.Buy Potion(20)\n0.Exit\n",player.gold);
        scanf("%d",&c);
        if(c==1){
            if(player.gold>=20){
                player.gold-=20;
                player.potions++;
            }else printf("Not enough gold!\n");
        }
    }while(c!=0);
}

void saveGame(){
    FILE *f=fopen("save.txt","w");
    if(f){
        fprintf(f,"%s %d %d %d %d %d %d %d %d %d",player.name,player.hp,player.maxHp,player.mp,player.maxMp,player.attack,player.level,player.exp,player.gold,player.potions);
        fclose(f);
        printf("Saved!\n");
    }
}

void loadGame(){
    FILE *f=fopen("save.txt","r");
    if(!f){ printf("No save found.\n"); createCharacter(); return; }
    fscanf(f,"%29s %d %d %d %d %d %d %d %d %d",player.name,&player.hp,&player.maxHp,&player.mp,&player.maxMp,&player.attack,&player.level,&player.exp,&player.gold,&player.potions);
    fclose(f);
}

void gameMenu(){
    int c;
    do{
        printf("\n1.Battle\n2.Shop\n3.Stats\n4.Save\n5.Exit\nChoice: ");
        scanf("%d",&c);
        if(c==1) battle();
        else if(c==2) shop();
        else if(c==3) showStats();
        else if(c==4) saveGame();
        else if(c==5) printf("Thanks for playing!\n");
        else printf("Invalid choice!\n");
    }while(c!=5);
}
