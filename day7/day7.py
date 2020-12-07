#!/usr/bin/env python3

import collections
import re

BAG_PATTERN = re.compile(r"(\d+) (.*)")
SPLIT_PATTERN = re.compile(r" bags contain | bags?, | bags?.")

def parse_graph(txt):
    forward = collections.defaultdict(lambda: [])
    backwards = collections.defaultdict(lambda: [])
    for line in txt.split("\n"):
        lst = SPLIT_PATTERN.split(line)
        name = lst.pop(0)
        for bag in lst:
            m = BAG_PATTERN.match(bag)
            if m:
                forward[name].append((m[2], int(m[1])))
                backwards[m[2]].append((name, int(m[1])))
    return forward, backwards

def dfs(graph, start, visited, stack):
    visited.add(start)
    for color, weight in graph[start]:
        if not color in visited:
            dfs(graph, color, visited, stack)
    stack.append(start)

def containing_colors(graph, color):
    stack = []
    dfs(graph, color, set(), stack)
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

f, b = parse_graph(txt)
assert containing_colors(b, "shiny gold") == 4
assert contained_bags(f, "shiny gold") == 32

with open("input", "rt") as f:
    f, b = parse_graph(f.read())

print("Part1:", containing_colors(b, "shiny gold"))
print("Part2:", contained_bags(f, "shiny gold"))
