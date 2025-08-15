# B+Tree Implementation in C

## Quick Start

```bash
git clone https://github.com/Noxy3301/bptree-c.git
cd bptree-c
./bptree
```

## Usage

| Command | Description | Example |
|---------|-------------|---------|
| `add <key>` | Insert key into tree | `add 10` |
| `del <key>` | Delete key from tree | `del 10` |
| `scan` | Print all keys in order | `scan` |
| `range <start> <end>` | Print keys in range <br> (inclusive) | `range 5 20` |
| `exit` | Quit program | `exit` |

## Example

```bash
noxy@XG6326-2U:~/work/bptree-c$ ./bptree
Usage: add <key> | del <key> | scan | range <start> <end> | exit
> add 10
[10]
--------------------------------------
> add 30
[10 30]
--------------------------------------
> add 60
[10 30 60]
--------------------------------------
> add 1111
[[10 30]60[60 1111]]
--------------------------------------
> add 88
[[10 30]60[60 88 1111]]
--------------------------------------
> add 62
[[10 30]60[60 62]88[88 1111]]
--------------------------------------
> del 60
[[10 30 62]88[88 1111]]
--------------------------------------
> del 88
[[10 30]62[62 1111]]
--------------------------------------
> scan
[ 10 30 62 1111 ]
--------------------------------------
> range 1 100
[ 10 30 62 ]
--------------------------------------
```