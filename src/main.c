#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include <stdbool.h>
#include <stdlib.h>

int last_frame_time = 0;
int game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;



struct {
  float x;
  float y;
  float width;
  float height;
  float vel_x;
  float vel_y;
} ball, paddle;

int initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL\n");
    return false;
  }

  window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    0
    //SDL_WINDOW_BORDERLESS
  );

  if (!window) {
    fprintf(stderr, "Error creating SDL Window\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if(!renderer) {
    fprintf(stderr, "Error creating SDL Renderer\n");
    return false;
  }

  return true;
}

void destroy_window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
    break;
    case SDL_KEYDOWN:
      if(event.key.keysym.sym == SDLK_ESCAPE) {
        game_is_running = false;
      }

      if (event.key.keysym.sym == SDLK_LEFT) {
        paddle.vel_x = -350;
      }
      if (event.key.keysym.sym == SDLK_RIGHT) {
        paddle.vel_x = 350;
      }
    break;
    case SDL_KEYUP:
      if (event.key.keysym.sym == SDLK_LEFT) {
        paddle.vel_x = 0;
      }
      if (event.key.keysym.sym == SDLK_RIGHT) {
        paddle.vel_x = 0;
      }
    break;
  }
}

void setup() {
  ball.x = WINDOW_WIDTH - 200;
  ball.y = 20;
  ball.width = 15;
  ball.height = 15;
  ball.vel_x = 250;
  ball.vel_y = 250;

  paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
  paddle.y = WINDOW_HEIGHT - 40;
  paddle.width = 100;
  paddle.height = 20;
  paddle.vel_x = 0;
  paddle.vel_y = 0;
}

void update() {
  float time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
  if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }


  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
  last_frame_time = SDL_GetTicks();

  ball.x += ball.vel_x * delta_time;
  ball.y += ball.vel_y * delta_time;
  
  paddle.x += paddle.vel_x * delta_time;

  if (paddle.x <= 0) {
    paddle.vel_x = 0;
    paddle.x = 0;
  }
  if ((paddle.x + paddle.width) >= WINDOW_WIDTH) {
    paddle.vel_x = 0;
    paddle.x = WINDOW_WIDTH - paddle.width;
  }
  
  if (ball.x <= 0) {
    ball.vel_x *= -1;
    ball.x = 0;
  }

  if ((ball.x + ball.width) >= WINDOW_WIDTH) {
    ball.vel_x *= -1;
    ball.x = WINDOW_WIDTH - ball.width;
  }

  if ((ball.y + ball.height) >= WINDOW_HEIGHT) {
    ball.x = (WINDOW_WIDTH / 2) - (ball.width / 2);
    ball.y = 20;
    ball.vel_x = 200;
    ball.vel_y = 200;
  }
  
  if (ball.y <= 0) {
    ball.vel_y *= -1;
    ball.y = 0;
  }

  if((ball.y + ball.height) >= paddle.y) {
    if((ball.x + ball.width) >= paddle.x && (ball.x) <= (paddle.x + paddle.width)) {
      ball.vel_y *= -1;
      ball.y = paddle.y - ball.height;
      ball.vel_x *= 1.1f;
      ball.vel_y *= 1.1f;
    }
  }

}

void draw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw rectangle
  SDL_Rect ball_rect = {
    (int) ball.x, 
    (int) ball.y, 
    (int) ball.width, 
    (int) ball.height 
  };
  
  SDL_Rect paddle_rect = {
    (int) paddle.x, 
    (int) paddle.y, 
    (int) paddle.width, 
    (int) paddle.height 
  };

  SDL_SetRenderDrawColor(renderer, 255,255, 255, 255);
  SDL_RenderFillRect(renderer, &ball_rect);
  SDL_RenderFillRect(renderer, &paddle_rect);

  SDL_RenderPresent(renderer);
}

int main() {
  game_is_running = initialize_window();

  setup();

  while(game_is_running) {
    process_input();
    update();
    draw();
  }

  destroy_window();
  
  return 0;
}
