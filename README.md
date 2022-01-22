# replace-multiple-words
Given replace map, we can replace all the correspond words in input file without wrong twice replace.

## Example:
### mapping.txt
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
2022紅色辣年糕
```
### output.txt
```
This is my Value.
我有一個綠色青椒
我還有草莓口味的小熊軟糖
2022草莓口味辣年糕
```
>In the second line, "紅色辣椒" would not be "草莓口味辣椒" and be "綠色青椒" correctly.
>In the fourth line, replace "紅色" with "草莓口味" successfully and would not be disturbed by "紅色辣椒".

## Getting started

```bash
git clone https://github.com/Zhima-Mochi/replace-multiple-words.git
cd replace-multiple-words
docker build -t replace-words .
cd test
docker run --rm -v ${PWD}:/files replace-words your_mapping_file.txt your_input_file.txt your_output_file.txt
```