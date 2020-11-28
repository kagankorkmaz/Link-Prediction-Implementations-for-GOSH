import networkx as nx
import time

deneme_f = open("facebook_combined_c.graph", "rb")
denemeG = nx.read_edgelist(deneme_f)
deneme_f.close()

# gosh_deneme_rai_f = open("./gosh_deneme_rai.txt", "r")
# gosh_deneme_rai = gosh_deneme_rai_f.read().splitlines()
# gosh_deneme_rai_arr = []
# # print(gosh_deneme_rai)
# # for line in gosh_deneme_rai:
# #     lineArr = line.split(" ")
# #     if lineArr[2] == "0":
# #         lineArr[2] = int(lineArr[2])
# #     else:
# #         lineArr[2] = float(lineArr[2])
# #     gosh_deneme_rai_arr.append((str(lineArr[0]), str(lineArr[1]), lineArr[2]))



# # print(gosh_deneme_rai_arr)
t0 = time.clock()
denemeG_rai = list(nx.resource_allocation_index(denemeG))
t1= time.clock()
print str(t1-t0) + " seconds"

# # print(set(gosh_deneme_rai_arr) == set(denemeG_rai))

# facebook_f = open("../facebook_combined_c.graph", "rb")
# facebook_g = nx.read_edgelist(facebook_f)
# facebook_f.close()
# facebook_rai = open("./facebook_rai_res.txt", "w")
# for t in list(nx.resource_allocation_index(facebook_g)):
#     facebook_rai.write(" ".join(str(s) for s in t) + "\n")

# facebook_rai.close()
# print("done")