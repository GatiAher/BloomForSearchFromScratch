"""
extract and save the text from 'explain xkcd' html pages

for each comic, parse the explained xkcd html file to save 
title, alt-text, and transcript as a single line to a file 
named "bitslicedsig/demo/xkcd_corpus/xkcd_" + comic_number + ".txt"

takes 20-30 minutes to run

@author: Gati Aher
"""

import argparse

# save
import json
# parse html
import re
import string
import requests
import bs4
# visualize progress bar in terminal
from tqdm import tqdm

# define save locations
raw_data_save_location = "bitslicedsig/demo/xkcd_corpus/xkcd_"
error_save_location = "bitslicedsig/demo/xkcd_corpus/xkcd_"


def put_text_into_file(comic_number):
    """
    For each comic number, scrape info from explain xkcd page
    Save title + alt-text + transcript in file

    Args:
        comic_number (positive integer): comics are numbered sequentially

    Return:
        alt_string (string):  alt-text, written text supplementary to comic         
    """

    # get HTML text
    text = requests.get(
        'https://www.explainxkcd.com/wiki/index.php/' + str(comic_number))
    if text.status_code != 200:
        raise Exception(str(text.status_code) +
                        " could not find explainxkcd page")
    soup = bs4.BeautifulSoup(text.text, 'html5lib')

    # get title
    title = soup.title.string[:-15]
    number_string = re.search(r'\d+', title).group()
    name_string = title[2 + title.index(":"):]

    # get alt-text
    alt = soup.find("a", {"class": "image"})
    alt_string = alt.get('title')

    # get transcript
    # add all transcript lines to list
    transcript_lines = []
    # span element with id Transcript is a level below
    # header element that is siblings with dl element
    transcript_element = soup.find("span", {"id": "Transcript"})
    if transcript_element is not None:
        # NOTE: comic 1116 doesn't have transcript
        current_element = transcript_element.parent
        # append all the dd elements
        while(current_element.next_sibling is not None):
            current_element = current_element.next_sibling
            # print(current_element)
            if current_element.name == 'span' or current_element.name == 'h2':
                break
            for item in current_element:
                if(type(item) == bs4.element.Tag):
                    transcript_lines.append(item.text)
    transcript_string = " ".join(transcript_lines)

    # save into file
    print("NUMBER: ", number_string)
    save_string = name_string + " "
    if alt_string is not None:
        save_string += alt_string + " "
    save_string += transcript_string
    print("SAVE STRING: ", save_string)

    file1 = open(raw_data_save_location + number_string + ".txt", "w")
    file1.write(save_string)
    file1.close()

    return alt_string


if __name__ == "__main__":
    """record all comics in range 0-num_comics. record any errors"""

    parser = argparse.ArgumentParser()
    parser.add_argument("--start_index", type=int, default=0,
                        help="index to start counting comics from, default is 0")
    parser.add_argument("--end_index", type=int, default=10,
                        help="index to stop counting comics from, default is 10")
    args = parser.parse_args()

    indexfile = open(raw_data_save_location + "index.txt", "w")

    for i in tqdm(range(args.start_index, args.end_index)):
        try:
            put_text_into_file(i)
            indexfile.write(
                str(i) + " " + raw_data_save_location + str(i) + ".txt\n")
        except Exception as err:
            # record errors
            f = open(error_save_location + str(i) + ".txt", "w")
            f.write("(" + str(i) + ") An exception occurred: ")
            f.write(" ".join(err.args))
            f.close()

    indexfile.close()
