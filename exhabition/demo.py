# -*- coding: utf-8 -*-

import pygame, Buttons, sys, os
from pygame.locals import *


pygame.init()

screen = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
pygame.display.set_caption('Monkey Fever')
pygame.display.set_caption('בנה פסל משלך')


model_image = pygame.image.load("./images/ego_geodesic_sphere_3_discrete_earth.jpg")

# screen.blit(monkey_surface, (0,0)) 
# pygame.display.flip()

# Fill background
background = pygame.Surface(screen.get_size())
background = background.convert()
background.fill((250, 250, 250))

text_color = (107,142,35)

# Display some text
font = pygame.font.Font(None, 36)
text = font.render("Please pick your model:", 1, text_color)
textpos = text.get_rect()
textpos.centerx = background.get_rect().centerx
background.blit(text, textpos)

# Blit everything to the screen
screen.blit(background, (0, 0))
pygame.display.flip()

# Buttons
button = Buttons.Button()
button.create_button(screen, text_color, 100, 100, 300, 300, 0, \
                         "World Example", (255,255,255), model_image)
pygame.display.flip()


def input(events): 
   for event in events: 
      if event.type == QUIT: 
         sys.exit(0)
      elif event.type == KEYUP:
         if event.key == K_ESCAPE:
            sys.exit(0)
      elif event.type == MOUSEBUTTONDOWN:
         if button.pressed(pygame.mouse.get_pos()):
             os.system("player ../data/ego/ego_geodesic_sphere_3_128.wrl")


while True: 
   input(pygame.event.get())
