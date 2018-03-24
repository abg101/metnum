
def compara( x, y ) :
    # x e y son objetos de los que se desea ordenar
    if x[0] > y[0] :
      rst = -1
    elif x[0] < y[0] :
      rst = 1
    else :
      rst = 0

    return rst

equipos = open("equipos.txt", "r") 
rankings = open("nba_2016_scores_CMM.txt", "r")
eqYrank = list(range(0, 30))
i = 0
while  i < 30:
	eqYrank[i] = (rankings.readline(), equipos.readline()) 
	i = i + 1 

eqYrank.sort(compara)
print eqYrank



