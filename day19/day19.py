#!/usr/bin/env python3

class Symbol(object):
    def __init__(self, terminal, value):
        self.terminal = terminal
        self.value = value

class Grammar(object):
    def __init__(self):
        self.productions = {}
        self.patched = False

    def parse(self, txt):
        self.productions.clear()
        rows = []
        state = 0
        for line in txt.strip().split("\n"):
            if state == 0:
                if line.strip() == "":
                    state = 1
                    continue

                pid, rest = line.split(": ")
                pid = int(pid)
                lst = []
                for r in rest.split(" | "):
                    if r[0] != "\"":
                        lst.append([Symbol(False, int(x)) for x in r.split(" ")])
                    else:
                        lst.append([Symbol(True, r[1])])
                    self.productions[pid] = lst
            else:
                rows.append(line)

        return rows

    def count_valid_rows(self, rows, patched):
        self.patched = patched
        return sum(1 for row in rows if self.is_valid(row))

    def is_valid(self, txt):
        self.pos = 0
        self.txt = txt
        return self.match(0) and self.pos == len(self.txt)

    def match(self, pid):
        if self.pos >= len(self.txt):
            return False

        tmp = self.pos
        for rule in self.productions[pid]:
            result = True
            for s in rule:
                if s.terminal:
                    if self.txt[self.pos] != s.value:
                        result = False
                        break
                    else:
                        self.pos += 1
                elif self.patched and s.value == 8:
                    if not self.match(42):
                        result = False
                        break
                elif self.patched and s.value == 11:
                    if not self.match(42):
                        result = False
                        break
                    cnt = 0
                    while self.match(42):
                        cnt += 1
                    if not self.match(31):
                        result = False
                        break
                    while self.match(31):
                        cnt -= 1
                    if cnt < 0:
                        result = False
                        break
                elif not self.match(s.value):
                    result = False
                    break

            if result:
                return result

            # backtrack and start another rule
            self.pos = tmp

        return False

txt ="""0: 4 1 5
1: 2 3 | 3 2
2: 4 4 | 5 5
3: 4 5 | 5 4
4: "a"
5: "b"

ababbb
bababa
abbbab
aaabbb
aaaabbb"""
g = Grammar()
rows = g.parse(txt)
assert g.count_valid_rows(rows, False) == 2
rows = """aaaabb
aaabab
abbabb
abbbab
aabaab
aabbbb
abaaab
ababbb""".split("\n")
assert g.count_valid_rows(rows, False) == 8

txt = """42: 9 14 | 10 1
9: 14 27 | 1 26
10: 23 14 | 28 1
1: "a"
11: 42 31
5: 1 14 | 15 1
19: 14 1 | 14 14
12: 24 14 | 19 1
16: 15 1 | 14 14
31: 14 17 | 1 13
6: 14 14 | 1 14
2: 1 24 | 14 4
0: 8 11
13: 14 3 | 1 12
15: 1 | 14
17: 14 2 | 1 7
23: 25 1 | 22 14
28: 16 1
4: 1 1
20: 14 14 | 1 15
3: 5 14 | 16 1
27: 1 6 | 14 18
14: "b"
21: 14 1 | 1 14
25: 1 1 | 1 14
22: 14 14
8: 42
26: 14 22 | 1 20
18: 15 15
7: 14 5 | 1 21
24: 14 1

abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa
bbabbbbaabaabba
babbbbaabbbbbabbbbbbaabaaabaaa
aaabbbbbbaaaabaababaabababbabaaabbababababaaa
bbbbbbbaaaabbbbaaabbabaaa
bbbababbbbaaaaaaaabbababaaababaabab
ababaaaaaabaaab
ababaaaaabbbaba
baabbaaaabbaaaababbaababb
abbbbabbbbaaaababbbbbbaaaababb
aaaaabbaabaaaaababaa
aaaabbaaaabbaaa
aaaabbaabbaaaaaaabbbabbbaaabbaabaaa
babaaabbbaaabaababbaabababaaab
aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba"""
rows = g.parse(txt)
assert g.count_valid_rows(rows, False) == 3
assert g.count_valid_rows(rows, True) == 12

with open("input", "rt") as f:
    rows = g.parse(f.read())

print("Part1:", g.count_valid_rows(rows, False))
print("Part2:", g.count_valid_rows(rows, True))
