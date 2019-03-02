// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <random>

const int   thickness = 15;
const float paddleH   = 100.0f;

Game::Game()
    : mWindow(nullptr),
      mRenderer(nullptr),
      mTicksCount(0),
      mIsRunning(true),
      mPaddleDirL(0),
      mPaddleDirR(0) {}

bool Game::Initialize() {
  // Initialize SDL
  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if (sdlResult != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  // Create an SDL Window
  mWindow =
      SDL_CreateWindow("Game Programming in C++ (Chapter 1)",  // Window title
                       100,   // Top left x-coordinate of window
                       100,   // Top left y-coordinate of window
                       1024,  // Width of window
                       768,   // Height of window
                       0      // Flags (0 for no flags set)
      );

  if (!mWindow) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  //// Create SDL renderer
  mRenderer =
      SDL_CreateRenderer(mWindow,  // Window to create renderer for
                         -1,       // Usually -1
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!mRenderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }
  //
  mPaddlePosL.x = 10.0f;
  mPaddlePosR.x = 1024.0f - 10.0f - thickness;
  mPaddlePosL.y = 768.0f / 2.0f;
  mPaddlePosR.y = 768.0f / 2.0f;

  std::random_device              seed_gen;
  std::default_random_engine      engine(seed_gen());
  std::uniform_int_distribution<> dist_ball_count(1, 4);
  std::uniform_int_distribution<> dist_zero_one(0, 1);
  std::uniform_int_distribution<> dist_vel_abs(160, 400);
  const int                       ball_count = dist_ball_count(engine);
  for (int i = 0; i < ball_count; ++i) {
    float posX = 1024.0f / 2.0f;
    float posY = 768.0f / 2.0f;
    float velX =
        static_cast<float>(dist_vel_abs(engine) *
                           (dist_zero_one(engine) == 1 ? 1 : -1) / ball_count);
    float velY =
        static_cast<float>(dist_vel_abs(engine) *
                           (dist_zero_one(engine) == 1 ? 1 : -1) / ball_count);
    Ball ball{posX, posY, velX, velY};
    mBalls.push_back(ball);
  }
  return true;
}

void Game::RunLoop() {
  while (mIsRunning) {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      // If we get an SDL_QUIT event, end loop
      case SDL_QUIT:
        mIsRunning = false;
        break;
    }
  }

  // Get state of keyboard
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  // If escape is pressed, also end loop
  if (state[SDL_SCANCODE_ESCAPE]) {
    mIsRunning = false;
  }

  // Update paddle direction based on W/S keys
  mPaddleDirL = 0;
  if (state[SDL_SCANCODE_W]) {
    mPaddleDirL -= 1;
  }
  if (state[SDL_SCANCODE_S]) {
    mPaddleDirL += 1;
  }

  mPaddleDirR = 0;
  if (state[SDL_SCANCODE_I]) {
    mPaddleDirR -= 1;
  }
  if (state[SDL_SCANCODE_K]) {
    mPaddleDirR += 1;
  }
}

void Game::UpdateGame() {
  // Wait until 16ms has elapsed since last frame
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

  // Delta time is the difference in ticks from last frame
  // (converted to seconds)
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

  // Clamp maximum delta time value
  if (deltaTime > 0.05f) {
    deltaTime = 0.05f;
  }

  // Update tick counts (for next frame)
  mTicksCount = SDL_GetTicks();

  // Update paddle position based on direction
  if (mPaddleDirL != 0) {
    mPaddlePosL.y += mPaddleDirL * 800.0f * deltaTime;
    // Make sure paddle doesn't move off screen!
    if (mPaddlePosL.y < (paddleH / 2.0f + thickness)) {
      mPaddlePosL.y = paddleH / 2.0f + thickness;
    } else if (mPaddlePosL.y > (768.0f - paddleH / 2.0f - thickness)) {
      mPaddlePosL.y = 768.0f - paddleH / 2.0f - thickness;
    }
  }
  if (mPaddleDirR != 0) {
    mPaddlePosR.y += mPaddleDirR * 800.0f * deltaTime;
    // Make sure paddle doesn't move off screen!
    if (mPaddlePosR.y < (paddleH / 2.0f + thickness)) {
      mPaddlePosR.y = paddleH / 2.0f + thickness;
    } else if (mPaddlePosR.y > (768.0f - paddleH / 2.0f - thickness)) {
      mPaddlePosR.y = 768.0f - paddleH / 2.0f - thickness;
    }
  }

  // Update ball position based on ball velocity
  for (auto&& ball : mBalls) {
    ball.pos.x += ball.vel.x * deltaTime;
    ball.pos.y += ball.vel.y * deltaTime;
    // Bounce if needed
    // Did we intersect with the paddle?
    float diff = mPaddlePosL.y - ball.pos.y;
    // Take absolute value of difference
    diff = (diff > 0.0f) ? diff : -diff;
    if (
        // Our y-difference is small enough
        diff <= paddleH / 2.0f &&
        // We are in the correct x-position
        ball.pos.x <= 25.0f && ball.pos.x >= 20.0f &&
        // The ball is moving to the left
        ball.vel.x < 0.0f) {
      ball.vel.x *= -1.0f;
    }
    diff = mPaddlePosR.y - ball.pos.y;
    // Take absolute value of difference
    diff = (diff > 0.0f) ? diff : -diff;
    if (
        // Our y-difference is small enough
        diff <= paddleH / 2.0f &&
        // We are in the correct x-position
        (1024.0f - 25.0f) <= ball.pos.x && ball.pos.x <= (1024.0f - 20.0f) &&
        // The ball is moving to the right
        ball.vel.x > 0.0f) {
      ball.vel.x *= -1.0f;
    }
    // Did the ball go off the screen? (if so, end game)
    else if (ball.pos.x <= 0.0f || 1024.0f < ball.pos.x) {
      mIsRunning = false;
    }

    // Did the ball collide with the top wall?
    if (ball.pos.y <= thickness && ball.vel.y < 0.0f) {
      ball.vel.y *= -1;
    }
    // Did the ball collide with the bottom wall?
    else if (ball.pos.y >= (768 - thickness) && ball.vel.y > 0.0f) {
      ball.vel.y *= -1;
    }
  }
}

void Game::GenerateOutput() {
  // Set draw color to blue
  SDL_SetRenderDrawColor(mRenderer,
                         0,    // R
                         0,    // G
                         255,  // B
                         255   // A
  );

  // Clear back buffer
  SDL_RenderClear(mRenderer);

  // Draw walls
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

  // Draw top wall
  SDL_Rect wall{
      0,         // Top left x
      0,         // Top left y
      1024,      // Width
      thickness  // Height
  };
  SDL_RenderFillRect(mRenderer, &wall);

  // Draw bottom wall
  wall.y = 768 - thickness;
  SDL_RenderFillRect(mRenderer, &wall);

  // Draw paddle
  SDL_Rect paddle{static_cast<int>(mPaddlePosL.x),
                  static_cast<int>(mPaddlePosL.y - paddleH / 2), thickness,
                  static_cast<int>(paddleH)};
  SDL_RenderFillRect(mRenderer, &paddle);
  paddle.x = static_cast<int>(mPaddlePosR.x);
  paddle.y = static_cast<int>(mPaddlePosR.y - paddleH / 2);
  paddle.w = thickness;
  paddle.h = static_cast<int>(paddleH);
  SDL_RenderFillRect(mRenderer, &paddle);

  // Draw ball
  SDL_Rect rBall;
  for (const auto& ball : mBalls) {
    rBall.x = static_cast<int>(ball.pos.x - thickness / 2);
    rBall.y = static_cast<int>(ball.pos.y - thickness / 2);
    rBall.w = thickness;
    rBall.h = thickness;
    SDL_RenderFillRect(mRenderer, &rBall);
  }

  // Swap front buffer and back buffer
  SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}
