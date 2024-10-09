import random
import pygame
import sys
from copy import deepcopy
from time import time, sleep
from pprint import pprint

class Game:
    # Определяем основные параметры
    SCREEN_WIDTH = 500
    SCREEN_HEIGHT = 500
    FPS = 60
    COLORS = {
        0: (255, 255, 255, 50),
        1: (126,170,83),
        2: (208,202,16),
        3: (233,134,7),
        4: (254,91,0),
        5: (217,29,17),
        6: (169,28,44),
        7: (146,72,109),
        8: (141,112,170),
        9: (110,97,166),
        10: (41,81,203),
        11: (0, 0, 0) 
    }

    def __init__(self, field_side_size):
        # Инициализирую Pygame
        pygame.init()
        pygame.font.init()
        self.field_side_size = field_side_size
        # Создаю окно
        self.screen = pygame.display.set_mode((Game.SCREEN_WIDTH, Game.SCREEN_HEIGHT))
        pygame.display.set_caption("2048 Rustamov Radmir Б05-452")
        self.font = pygame.font.SysFont('serif', 72)
        # Устанавливаю таймер
        self.clock = pygame.time.Clock()
        
        self.field_side_size_range = range(self.field_side_size)
        self.zero_field()

        pprint(self.field)
        self.cool_down = 0
        
        self.cell_size = Game.SCREEN_WIDTH / self.field_side_size

        self.is_winner = False

        self.running = True

    def run(self):
        while self.running:
            self.handle_events()
            self.update()
            self.render()
            self.clock.tick(Game.FPS)

    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False

    def zero_field(self):
        self.field = [[0 for _ in self.field_side_size_range] for _ in self.field_side_size_range]
        self.field[random.choice(self.field_side_size_range)][random.choice(self.field_side_size_range)] = 1 # Начальная точка 


    @staticmethod
    def move_block(field, i, j, add_i, add_j):
        if field[i][j] and field[i+add_i][j+add_j]:
            if field[i][j] == field[i+add_i][j+add_j]:
                field[i+add_i][j+add_j] += 1
                field[i][j] = 0
            else:
                pass
        elif not field[i][j]:
            pass
        else:
            field[i+add_i][j+add_j] = field[i][j]
            field[i][j] = 0

    
    def move_right(self, field):
        last_iteration_field = []
        while last_iteration_field != field:
            last_iteration_field = deepcopy(field)
            for i in self.field_side_size_range:
                for j in range(self.field_side_size-2, -1, -1):
                    self.move_block(field, i, j, 0, 1)
        return field

    def move_left(self, field):
        last_iteration_field = []
        while last_iteration_field != field:
            last_iteration_field = deepcopy(field)
            for i in self.field_side_size_range:
                for j in range(1, self.field_side_size):
                    self.move_block(field, i, j, 0, -1)
        return field

    def move_up(self, field):
        last_iteration_field = []
        while last_iteration_field != field:
            last_iteration_field = deepcopy(field)
            for i in range(1, self.field_side_size):
                for j in self.field_side_size_range:
                    self.move_block(field, i, j, -1, 0)
        return field

    def move_down(self, field):
        last_iteration_field = []
        while last_iteration_field != field:
            last_iteration_field = deepcopy(field)
            for i in range(self.field_side_size-2, -1, -1):
                for j in self.field_side_size_range:
                    self.move_block(field, i, j, 1, 0)
        return field

    def key_event(self):
        pressed = pygame.key.get_pressed()
        field_before_move = deepcopy(self.field)
        
        if pressed[pygame.K_RIGHT]:
            self.move_right(self.field)

        elif pressed[pygame.K_LEFT]:
            self.move_left(self.field)

        elif pressed[pygame.K_UP]:
            self.move_up(self.field)

        elif pressed[pygame.K_DOWN]:
            self.move_down(self.field)
        
        if self.field == field_before_move:
            # ничего не поменялось
            pass
        else:
            # Начинается отсчёт кулдауна 0.2 секунды
            self.cool_down = time()

            # добавляется ещё один блок минимальной величины
            cords = random.choice(self.field_side_size_range), random.choice(self.field_side_size_range)
            while self.field[cords[0]][cords[1]]:
                # print(self.field[cords[0]][cords[1]], cords)
                cords = random.choice(self.field_side_size_range), random.choice(self.field_side_size_range)
            else:
                self.field[cords[0]][cords[1]] = 1
                # print(self.field[cords[0]][cords[1]])

            # Все ли клетки заполнены
            if all([all(i) for i in self.field]):
                field_before_check = deepcopy(self.field)
                checking_field = deepcopy(self.field)
                # Проверка поменяется ли что-то, если переместить блоки во все стороны
                if self.move_left(checking_field) == field_before_check and\
                    self.move_right(checking_field) == field_before_check and\
                    self.move_up(checking_field) == field_before_check and\
                    self.move_down(checking_field) == field_before_check:
                    pass
                else:
                    # Если нет, то очищение поля
                    print("Game Over")
                    self.zero_field()
                    sleep(3)

            # Если есть хотя бы одно число 11 (2 ** 11 = 2048), то победитель
            if any(11 in i for i in self.field):
                print("Winner!")
                # Не сразу перезапускаю поле, чтобы было время обновить экран и увидеть блок 2048
                self.is_winner = True
                
    def font_render(self):
        for i in self.field_side_size_range:
            for j in self.field_side_size_range:
                if self.field[i][j] != 0:
                    number = self.font.render(f'{self.field[i][j]}', False, (255, 0, 0))
                    self.screen.blit(number, (self.cell_size * j * 1.02 + self.cell_size * 0.3, self.cell_size * i * 1.02 + self.cell_size * 0.16))
            

    def update(self):
        for i in self.field_side_size_range:
            for j in self.field_side_size_range:
                pygame.draw.rect(self.screen,
                            Game.COLORS[self.field[i][j]],
                            pygame.Rect(
                                self.cell_size * j * 1.02,
                                self.cell_size * i * 1.02,
                                self.cell_size * 0.95,
                                self.cell_size * 0.95
                            ))

    def render(self):
        if self.is_winner:
            sleep(3)
            self.zero_field()
            self.is_winner = False
        # Очистка экрана
        self.screen.fill((200, 100, 0))
        if time() - self.cool_down >= 0.2:
            self.key_event()
        self.update()
        self.font_render()
        
        # Обновление экрана
        pygame.display.flip()

    def cleanup(self):
        pygame.quit()
        sys.exit()

if __name__ == "__main__":
    Game(4).run()
