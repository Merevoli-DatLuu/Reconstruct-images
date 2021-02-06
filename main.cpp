#include <bits/stdc++.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace std;
struct CL{
    int r, g, b;
    CL(int r, int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
};
sf::Image origin_image;
sf::Vector2u image_size;
string image_path = "img.png";

sf::Image reconstructed_image;

vector <vector <CL> > origin_matrix;
vector <vector <CL> > reconstructed_matrix;

int best_score = 0;
int origin_score = 0;

void init();
int getScore();
void insertShape(int x0, int y0, int x1, int y1, sf::Color color);

void init(){
    if (!origin_image.loadFromFile(image_path)){
        cout << "No Image" << '\n';
    }
    image_size = origin_image.getSize();
    reconstructed_image.create(image_size.x, image_size.y, sf::Color::Black);

    for (unsigned int x = 0; x<image_size.x; x++){
        vector <CL> v1, v2;
        for (unsigned int y = 0; y<image_size.y; y++){
            sf::Color pixel_ori = origin_image.getPixel(x, y);
            sf::Color pixel_rec = reconstructed_image.getPixel(x, y);
            v1.push_back(CL(pixel_ori.r, pixel_ori.g, pixel_ori.b));
            v2.push_back(CL(pixel_rec.r, pixel_rec.g, pixel_rec.b));
        }
        origin_matrix.push_back(v1);
        reconstructed_matrix.push_back(v2);
    }
}

/*
    Tổng RGB của pixel
*/
int getScore(){
    int score = 0;

    for (unsigned int x = 0; x<image_size.x; x++){
        for (unsigned int y = 0; y<image_size.y; y++){
            sf::Color pixel_ori = origin_image.getPixel(x, y);
            sf::Color pixel_rec = reconstructed_image.getPixel(x, y);
            score += abs(pixel_ori.r - pixel_rec.r) + abs(pixel_ori.b - pixel_rec.b) + abs(pixel_ori.g - pixel_rec.g);
        }
    }

    return score;
}

int getScore_2(){
    int score = 0;

    for (unsigned int x = 0; x<image_size.x; x++){
        for (unsigned int y = 0; y<image_size.y; y++){
            CL pixel_ori = origin_matrix[x][y];
            CL pixel_rec = reconstructed_matrix[x][y];
            score += abs(pixel_ori.r - pixel_rec.r) + abs(pixel_ori.b - pixel_rec.b) + abs(pixel_ori.g - pixel_rec.g);
        }
    }

    return score;
}

void insertShape(int x0, int y0, int x1, int y1, sf::Color color){
    for (int x=x0; x<=x1; x++){
        for (int y=y0; y<=y1; y++){
            reconstructed_image.setPixel(x, y, color);
        }
    }
}

void update(){
    int x0 = rand() % image_size.x;
    int y0 = rand() % image_size.y;

    float A = (float)best_score/origin_score;
    cout << A << '\n' ;
    int x1 = x0 + rand() % (image_size.x - x0);
    int y1 = y0 + rand() % (image_size.y - y0);

    sf::Image image = reconstructed_image;

    insertShape(x0, y0, x1, y1, sf::Color(rand()%255, rand()%255, rand()%255));

    if (best_score > getScore()){
        best_score = getScore();
    }
    else{
        reconstructed_image = image;
    }

}

void update_2(){
    int x0 = rand() % image_size.x;
    int y0 = rand() % image_size.y;

    int x1 = x0 + rand() % (image_size.x - x0);
    int y1 = y0 + rand() % (image_size.y - y0);

    sf::Image image = reconstructed_image;

    int r = 0;
    int g = 0;
    int b = 0;
    for (r=0; r<256; r++){
        insertShape(x0, y0, x1, y1, sf::Color(r, g, b));
        if (best_score > getScore()){
            best_score = getScore();
            image = reconstructed_image;
        }
    }
    reconstructed_image = image;

    for (g=0; g<256; g++){
        insertShape(x0, y0, x1, y1, sf::Color(r, g, b));
        if (best_score > getScore()){
            best_score = getScore();

            image = reconstructed_image;
        }
    }

    reconstructed_image = image;

    for (b=0; b<256; b++){
        insertShape(x0, y0, x1, y1, sf::Color(r, g, b));
        if (best_score > getScore()){
            best_score = getScore();

            image = reconstructed_image;
        }
    }
    reconstructed_image = image;

    cout << r << ' ' << g << ' ' << b << '\n';

}

void update_3(int id){
    while (true){
        update();
        cout << "Score: " << id << "-" << best_score << '\n';
    }
}

int main()
{
    init();

    best_score = getScore();
    origin_score = best_score;
    sf::RenderWindow window(sf::VideoMode(image_size.x, image_size.y), "SFML works!");

//    vector <sf::Thread> thread_ls;
//    for (int i=1; i<=100; i++){
//        sf::Thread thread(bind(&update_3, i));
//        thread.launch();
//        thread_ls.push_back(thread);
//    }
    int stt = 0;
    clock_t start = clock();
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Texture texture;
        texture.loadFromImage(reconstructed_image);  //Load Texture from image

        sf::Sprite sprite;
        sprite.setTexture(texture);

        window.clear();
        window.draw(sprite);
        window.display();
        for (int i=1; i<=10; i++){
            cout << "Score: " << ++stt << " " << best_score << '\n';
            update();
        }
    }
    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
//    for (sf::Thread thread : thread_ls){
//        thread.wait();
//    }

    return 0;
}
