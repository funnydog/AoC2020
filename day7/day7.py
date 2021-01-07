#!/usr/bin/env python3

import re
import sys

PATTERN = re.compile(r"(.*) bags contain|(\d+) ([^.,]*) bag")

def parse_graph(txt):
    graph = {}
    for line in txt.split("\n"):
        lst = PATTERN.findall(line)
        if not lst:
            continue
        name, _, _ = lst.pop(0)
        if not name:
            continue
        graph[name] = []
        for _, num, color in lst:
            graph[name].append((color, int(num)))
    return graph

def transpose(graph):
    newg = {}
    for key in graph.keys():
        newg[key] = []

    for v1, edges in graph.items():
        for v2, weight in edges:
            newg[v2].append((v1, weight))

    return newg

def dfs(graph, start, visited, stack):
    visited.add(start)
    for color, weight in graph[start]:
        if not color in visited:
            dfs(graph, color, visited, stack)
    stack.append(start)

def containing_colors(graph, color):
    stack = []
    dfs(transpose(graph), color, set(), stack)
    return len(stack)-1

def contained_bags(graph, color):
    stack = []
    dfs(graph, color, set(), stack)
    counts = {}
    for bag in stack:
        count = 0
        for col, num in graph[bag]:
            count += num * (counts[col] + 1)
        counts[bag] = count
    return counts[color]

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: {} <filename>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    try:
        with open(sys.argv[1], "rt") as f:
            txt = f.read()
    except:
        print("Cannot open {}".format(sys.argv[1]), file=sys.stderr)
        sys.exit(1)

    g = parse_graph(txt)
    print("Part1:", containing_colors(g, "shiny gold"))
    print("Part2:", contained_bags(g, "shiny gold"))
