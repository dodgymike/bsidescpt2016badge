import random

from clan import Clan
from clan_member import ClanMember
from fight import Fight

clan_red = Clan("red")
clan_green = Clan("green")
clan_blue = Clan("blue")

clans = [ clan_red, clan_green, clan_blue ]

num_players = 210
all_players = []

for x in range(0, num_players/3):
  red_clan_member = ClanMember("player_red_" + str(x), clan_red)
  green_clan_member = ClanMember("player_green_" + str(x), clan_green)
  blue_clan_member = ClanMember("player_blue_" + str(x), clan_blue)

  all_players.append(red_clan_member)
  all_players.append(green_clan_member)
  all_players.append(blue_clan_member)

print clan_red
print clan_green
print clan_blue

f=Fight()

for round_count in range(0, 10000):
  for x in range(0, num_players):
    player_a = random.choice(all_players)
    player_b = random.choice(all_players)
    f.fight(player_a, player_b)

  if round_count % 300 == 0:
    print "Round:" + str(round_count)
    print clan_red
    print clan_green
    print clan_blue

  # defections
  defector = random.choice(all_players)
  defector_new_clan = random.choice(clans)
  defector_new_clan.add_member(defector)

print clan_red
print clan_green
print clan_blue
