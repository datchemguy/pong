#include <sfml/graphics.hpp>
#include <time.h>
#include <iostream>
using namespace sf;

Font f;

void drawGame(RenderWindow &w, unsigned short y, unsigned short y2, double bx, double by, unsigned short a, unsigned short b){
    ConvexShape pad1(4);
    pad1.setFillColor(Color::Green);
    pad1.setPoint(0, Vector2f(64, y-45));
    pad1.setPoint(1, Vector2f(64, y+45));
    pad1.setPoint(2, Vector2f(80, y+45));
    pad1.setPoint(3, Vector2f(80, y-45));
    ConvexShape pad2(4);
    pad2.setFillColor(Color::Green);
    pad2.setPoint(0, Vector2f(1216, y2-45));
    pad2.setPoint(1, Vector2f(1216, y2+45));
    pad2.setPoint(2, Vector2f(1200, y2+45));
    pad2.setPoint(3, Vector2f(1200, y2-45));
    CircleShape ball;
    ball.setFillColor(Color::Yellow);
    ball.setPosition(bx-16, by-16);
    ball.setRadius(16);
    Text score;
    score.setString(std::to_string(a) + "-" + std::to_string(b));
    score.setFont(f);
    score.setCharacterSize(36);
    score.setPosition(640-(score.getLocalBounds().width/2), 36);
    w.draw(pad1); w.draw(pad2); w.draw(ball); w.draw(score);
}

int main(){
    RenderWindow app(VideoMode(1280,720), "Pong");
    app.setFramerateLimit(60);
    unsigned short pad1pos = 360, pad2pos = 360, score1 = 0, score2 = 0;
    unsigned int seed = time(NULL)*4711/11%1000, s = time(NULL);
    double ballx = 640, bally = 360, angle = seed/14.0;
    short diry = 2*(seed%2)-1, dirx = 2*(s%2)-1;
    bool on = false;
    if(!f.loadFromFile("unifont.ttf")){}

    drawGame(app, 360, 360, 640, 360, 0, 0);
    Text st, pl;
    st.setString("Space - Start | P - Pause");
    pl.setString("P1: W/S | P2: Up/Down");
    st.setFont(f); pl.setFont(f);
    st.setCharacterSize(60); pl.setCharacterSize(60);
    st.setPosition(640-(st.getLocalBounds().width/2), 520);
    pl.setPosition(640-(pl.getLocalBounds().width/2), 620);
    app.draw(st); app.draw(pl);
    app.display();

    while(app.isOpen()){
        Event e;
        while(app.pollEvent(e)){
            if(e.type==Event::Closed) app.close();
        }

        seed = time(NULL)*4311/11%1000;

        if(Keyboard::isKeyPressed(Keyboard::Space)) on = true; //start
        if(Keyboard::isKeyPressed(Keyboard::P)) on = false; //pause

        if(on){
            app.clear();

            if(Keyboard::isKeyPressed(Keyboard::W)) pad1pos -= 9; //move up
            if(pad1pos<45) pad1pos = 45; //limit pad up

            if(Keyboard::isKeyPressed(Keyboard::S)) pad1pos += 9; //move down
            if(pad1pos>675) pad1pos = 675; //limit pad down

            if(Keyboard::isKeyPressed(Keyboard::Up)) pad2pos -= 9;
            if(pad2pos<45) pad2pos = 45;

            if(Keyboard::isKeyPressed(Keyboard::Down)) pad2pos += 9;
            if(pad2pos>675) pad2pos = 675;

            if(ballx >= 80 && ballx <= 96 && bally >= pad1pos-61 && bally <= pad1pos+61 && dirx == -1){ //pad collision
                dirx = 1;
                angle = abs(bally-pad1pos)+(seed/100.0)-5;
                diry = 2*(bally-pad1pos >= 0)-1;
            }
            else if(ballx <= 1200 && ballx >= 1184 && bally >= pad2pos-61 && bally <= pad2pos+61 && dirx == 1){ //pad2 collision
                dirx = -1;
                angle = abs(bally-pad2pos)+(seed/100.0)-5;
                diry = 2*(bally-pad2pos >= 0)-1;
            }

            if(bally < 16) bally = 16;
            if(bally == 16) diry = 1; //up wall
            if(bally > 704) bally = 704;
            if(bally == 704) diry = -1; //down wall
            if(ballx < 16) ballx = 16;
            if(ballx == 16) {dirx = 1; score2++;} //left wall
            if(ballx > 1264) ballx = 1264;
            if(ballx == 1264) {dirx = -1; score1++;} //right wall

            //ball move
            ballx += dirx*16;
            bally += diry*(angle/6.0);

            drawGame(app, pad1pos, pad2pos, ballx, bally, score1, score2);
            app.display();
        }
    }
    return 0;
}