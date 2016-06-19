class Clan:
  def __init__(self, colour):
    self.clan_members = []
    self.colour = colour

  def clan_members(self):
    return list(self.clan_members)

  def add_member(self, clan_member):
    self.clan_members.append(clan_member)

  def colour():
    return self.colour

  def __repr__(clan):
    return "Clan: colour={0},members={1}".format(clan.colour, clan.clan_members)

