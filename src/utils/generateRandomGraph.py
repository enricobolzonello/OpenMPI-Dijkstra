import networkx as nx
import argparse
import os
import random

def generateGraph():
    G = nx.fast_gnp_random_graph()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate random graphs and save it to file as an edge list')
    parser.add_argument('-p', type=float, metavar='P',default=0.5, help='probability for edge creation. Float value between 0 and 1')
    parser.add_argument('-n', type=int, metavar='N',required=True, help='number of nodes')
    parser.add_argument('-o', metavar='dir',required=True, help='output directory')
    args = parser.parse_args()

    if not os.path.isdir(args.o):
        raise ValueError(f"{args.o} is not a directory")

    if args.n <= 0:
        raise ValueError("Number of nodes must be greater than 0")
    
    if args.p <= 0.0 or args.p > 1.0:
        raise ValueError("Probability should be between 0 and 1")

    G = nx.fast_gnp_random_graph(args.n, args.p)

    for (u, v) in G.edges():
        G.edges[u,v]['weight'] = random.randint(1,50)

    l = filter(lambda x: "__" not in x and ".txt" in x, os.listdir(args.o))
    l = list(l)
    n = int(sorted(l)[-1][6:8]) + 1 if len(l) > 0 else 1

    path = f"{args.o}/random{n:02d}.txt"
    with open(path, "w") as f:
        f.write(f"nodes: {args.n} edges: {G.size()}\n")

        for u,v,w in G.edges.data("weight"):
            f.write(f"{u}\t{v}\t{w}\n")

    print(f"Random graph generated in {path}")