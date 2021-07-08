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
啊:[del]
```
>Can use ":" or "：".  
>Key and value should not be "".  
>Use [del] to replace key as "".

### input.txt
```
This is my Key.
我有一個紅色辣椒啊
我還有紅色的草莓啊
```
### output.txt
```
This is my Value.
我有一個綠色青椒
我還有草莓口味的小熊軟糖

```
>In the second line, "紅色辣椒" would not be "草莓口味辣椒" and be "綠色青椒" correctly.