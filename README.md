# Coloretto-Game

It is a game of a board game named Coloretto.

The game allow a player plays Coloretto with 3 virtual opponents.
It uses only basic rules.

There are 77 cards including:

	Each color have 9 colorettos (7x9=63)
	There are 10 “+2” card (the score +2)
	There are 3 colorful colorettos (can act any color)
	1 end cards, put at the no 16 counter count.

In each round, the player can only choose:

	Draw a coloretto and put at some row
	Get the coloretto  on a row and end this round

Each row can only put 3 colorettos and each row can only get by one player at each round.
If someone draw a end card, then after this round we will calculate the score.

Score calculation:
Sort the count of each colors, the top 3 colors get positive points, others get minus points.

The different count get different point:

	1 : 1
	2 : 3
	3 : 6
	4 : 10
	5 : 15
	6 or more : 21

For example the sorted count is 654321, +2card 2, colorful 1, the score is 21+21+10 - 6 - 3 - 1+2x2.

The highest score player is winner!
