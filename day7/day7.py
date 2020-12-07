#!/usr/bin/env python3

import re

BAG_PATTERN = re.compile(r"(\d+) (.*)")
SPLIT_PATTERN = re.compile(r" bags contain | bags?, | bags?.")

def parse_graph(txt):
    graph = {}
    for line in txt.split("\n"):
        lst = SPLIT_PATTERN.split(line)
        name = lst.pop(0)
        graph[name] = []
        for bag in lst:
            m = BAG_PATTERN.match(bag)
            if m:
                graph[name].append((m[2], int(m[1])))
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

txt = """light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags."""

g = parse_graph(txt)
assert containing_colors(g, "shiny gold") == 4
assert contained_bags(g, "shiny gold") == 32

with open("input", "rt") as f:
    g = parse_graph(f.read())

print("Part1:", containing_colors(g, "shiny gold"))
print("Part2:", contained_bags(g, "shiny gold"))
