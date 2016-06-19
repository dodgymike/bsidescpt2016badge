from clan import Clan
from clan_member import ClanMember

clan_red = Clan("red")
clan_green = Clan("green")
clan_blue = Clan("blue")

ClanMember("bob", clan_red)
ClanMember("mary", clan_green)

print clan_red
print clan_green
print clan_blue

