#include <iostream>
#include <vector>
#include <cmath>
#include <string>

extern "C"
{
	#include <raylib.h>
}

// This don't need to comment, so i don't will comment this
constexpr std::uint16_t
	WINDOW_WIDTH {480},
	WINDOW_HEIGHT {720},
	TABLE_SLOTS_H {8},
	TABLE_SLOTS_V {8},
	TABLE_SLOT_SIZE {50};

const char*
	WINDOW_CAPTION {"Chess War"};

class Pice
{
public:

private:
	
};

class Table
{
public:
	Table()
		: __pice {nullptr}
	{
		this->_createSlots();
	}

	virtual void addPiceAt(Pice *pice, int x, int y) noexcept
	{
		this->__slots[y][x] = pice;
	}

	virtual void update(Vector2 target) noexcept
	{
		const Vector2 CENTER = this->getCenter();

		int 
			slot_x = static_cast<int>(std::ceil((target.x - CENTER.x) / TABLE_SLOT_SIZE)) - 1,
			slot_y = static_cast<int>(std::ceil((target.y - CENTER.y) / TABLE_SLOT_SIZE)) - 1;

		DrawText((std::to_string(slot_x) + ", " + std::to_string(slot_y)).c_str(), 16, 16, 20, GREEN);

		if(slot_x >= 0 && slot_x < TABLE_SLOTS_H && slot_y >= 0 && slot_y < TABLE_SLOTS_V)
		{
			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				this->__pice = this->__slots[slot_y][slot_x];
				this->__slots[slot_y][slot_x] = nullptr;
			}
			if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && this->__pice)
			{
				this->__slots[slot_y][slot_x] = this->__pice;
				this->__pice = nullptr;
			}
		}
	}

	const Vector2 getCenter() const noexcept
	{
		const Vector2 SCREEN_SIZE = {
			static_cast<float>(GetScreenWidth()),
			static_cast<float>(GetScreenHeight())
		};

		const Vector2 CENTER = {
			SCREEN_SIZE.x / 2.f - (TABLE_SLOTS_H * TABLE_SLOT_SIZE) / 2.f,
			SCREEN_SIZE.y / 2.f - (TABLE_SLOTS_V * TABLE_SLOT_SIZE) / 2.f
		};

		return CENTER;
	}

	virtual void draw() const noexcept
	{
		const Vector2 CENTER = this->getCenter();
		const Vector2 MOUSE_POSITION = GetMousePosition();

		for(int i = 0; i < TABLE_SLOTS_H * TABLE_SLOTS_V; i++)
		{
			DrawRectangle(
					CENTER.x + TABLE_SLOT_SIZE * (i / TABLE_SLOTS_H),
					CENTER.y + TABLE_SLOT_SIZE * (i % TABLE_SLOTS_V), 
					TABLE_SLOT_SIZE, TABLE_SLOT_SIZE,
					(i + i / TABLE_SLOTS_V) % 2 == 0 ? WHITE : BLACK);
			
			if(this->__slots[i % TABLE_SLOTS_V][i / TABLE_SLOTS_H] != nullptr)
			{
				if(!this->__pice)
				{
					DrawRectangle(
							CENTER.x + TABLE_SLOT_SIZE * (i / TABLE_SLOTS_H) + 6,
							CENTER.y + TABLE_SLOT_SIZE * (i % TABLE_SLOTS_V) + 6, 
							TABLE_SLOT_SIZE - 12, TABLE_SLOT_SIZE - 12,
							RED);
				}
			}
		}

		if(this->__pice)
		{
			DrawRectangle(
					MOUSE_POSITION.x - 25,
					MOUSE_POSITION.y - 25, 
					TABLE_SLOT_SIZE - 12, TABLE_SLOT_SIZE - 12,
					RED);
		}
	}

	virtual ~Table()
	{
	}
protected:
	virtual void _createSlots() noexcept
	{
		this->__slots.resize(8, std::vector<Pice*>(8, nullptr));
	}

private:
	std::vector<std::vector<Pice*>> __slots;
	Pice *__pice;
};

int main(int, char *[])
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CAPTION);

	Pice pice;

	Table table;

	table.addPiceAt(&pice, 4, 4);

	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GRAY);

		table.draw();
		table.update(GetMousePosition());

		EndDrawing();
	}

	CloseWindow();

	return EXIT_SUCCESS;
}
