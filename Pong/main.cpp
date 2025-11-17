#include <iostream>
#include <raylib.h>

constexpr int WINDOW_WIDTH = 1920 / 2;
constexpr int WINDOW_HEIGHT = 1080 / 2;

class Player
{
private:
	Rectangle player{ 3, WINDOW_HEIGHT / 2 - 75.0 / 2, 10, 75 };
	Vector2 player_direction{};
	float player_speed{250.0f};
	friend class Ball;

public:
	Player() = default;

	void check_wall_collision()
	{
		if (player.y <= 4)
		{
			player.y = 4;
		}

		if (player.y >= (WINDOW_HEIGHT - 4) - 75)
		{
			player.y = (WINDOW_HEIGHT - 4) - 75;
		}
	}

	void update(float dt)
	{
		player_direction.y = static_cast<int>(IsKeyDown(KEY_DOWN)) - static_cast<int>(IsKeyDown(KEY_UP));

		player.y += dt * player_speed * player_direction.y;

		check_wall_collision();
	}

	void draw() const
	{
		DrawRectangleRec(player, WHITE);
	}
};

class CPU : public Player
{
private:
	Rectangle cpu{ WINDOW_WIDTH - 13, WINDOW_HEIGHT / 2 - 75.0 / 2, 10, 75 };
	Vector2 cpu_direction{};
	float cpu_speed{ 300.0f };
	friend class Ball;

public:
	CPU() = default;

	void update(Ball& ball, float dt);

	void draw() const
	{
		DrawRectangleRec(cpu, WHITE);
	}
};

class Ball
{
private:
	float ball_radius{10};
	Vector2 ball_position{ WINDOW_WIDTH / 2 ,WINDOW_HEIGHT / 2 };
	Vector2 ball_direction{ 1,-1 };
	float ball_speed{ 600 };
	friend class CPU;

public:
	Ball() = default;

	void check_wall_collision()
	{
		if (ball_position.y + ball_radius >= WINDOW_HEIGHT)
		{
			ball_direction.y = -1;
		}

		if (ball_position.y - ball_radius <= 0)
		{
			ball_direction.y = 1;
		}

		if (ball_position.x + ball_radius >= WINDOW_WIDTH)
		{
			ball_position = { WINDOW_WIDTH / 2 ,WINDOW_HEIGHT / 2 };
		}

		if (ball_position.x - ball_radius <= 0)
		{
			ball_position = { WINDOW_WIDTH / 2 ,WINDOW_HEIGHT / 2 };
		}
	}

	void check_player_collision(Player& p1, CPU& cpu)
	{
		if (CheckCollisionCircleRec(ball_position, ball_radius, p1.player))
		{
			ball_direction.x = 1;
			ball_speed += 20;
		}

		if (CheckCollisionCircleRec(ball_position, ball_radius, cpu.cpu))
		{
			ball_direction.x = -1;
			ball_speed += 20;
		}
	}

	void update(Player& p1, CPU& cpu, float dt)
	{
		check_wall_collision();
		check_player_collision(p1, cpu);

		ball_position.x += ball_speed * dt * ball_direction.x;
		ball_position.y += ball_speed * dt * ball_direction.y;
	}

	void draw() const
	{
		DrawCircleV(ball_position, ball_radius, WHITE);
	}
};

void CPU::update(Ball& ball, float dt)
{
	check_wall_collision();

	if (ball.ball_position.x >= WINDOW_WIDTH / 4.0 && ball.ball_position.y <= cpu.y + 75.0 / 2)
	{
		cpu_direction.y = -1;
		cpu.y += cpu_speed * dt * cpu_direction.y;
	}
	else if (ball.ball_position.x >= WINDOW_WIDTH / 4.0 && ball.ball_position.y >= cpu.y + 75.0 / 2)
	{
		cpu_direction.y = 1;
		cpu.y += cpu_speed * dt * cpu_direction.y;
	}
}

int main()
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong");

	Player p1{};
	CPU cpu{};
	Ball ball{};
	float dt{};

	while (!WindowShouldClose())
	{
		dt = GetFrameTime();
		p1.update(dt);
		ball.update(p1, cpu, dt);
		cpu.update(ball, dt);

		BeginDrawing();
		ClearBackground(BLACK);

		p1.draw();
		cpu.draw();
		DrawRectangle(WINDOW_WIDTH / 2, 0, 1, WINDOW_HEIGHT, WHITE);
		ball.draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}