# replace-multiple-words
Given replace map, we can replace all the correspond words in input file without wrong twice replace.

```
.\transword.exe map.txt input.txt output.txt  
```
## Example:
### map.txt
```
Key:Value
紅色:草莓口味
草莓:小熊軟糖
紅色辣椒:綠色青椒
```
>Can use ":" or "：".

### input.txt
```
This is my Key.
我有一個紅色辣椒
我還有紅色的草莓
```
### output.txt
```
This is my Value.
我有一個綠色青椒
我還有草莓口味的小熊軟糖

```
>In the second line, "紅色辣椒" would not be "草莓口味辣椒" and be "綠色青椒" correctly.