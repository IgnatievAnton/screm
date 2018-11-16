import telebot
import re
from telebot import types
import logging


API_TOKEN = 'token'

bot = telebot.TeleBot(API_TOKEN)
telebot.logger.setLevel(logging.DEBUG)
telebot.logger.setLevel(logging.DEBUG)


@bot.inline_handler(lambda query: query.query == 'test')
def query_text(inline_query):
    try:
        r = types.InlineQueryResultArticle('1', 'R1', types.InputTextMessageContent(" .Все привет2 ! "))
        r2 = types.InlineQueryResultArticle('2', 'Result2', types.InputTextMessageContent('hi'))
        bot.answer_inline_query(inline_query.id, [r, r2])
    except Exception as e:
        print(e)




@bot.inline_handler(lambda query: len(query.query) is 0)
def default_query(inline_query):
    try:
        r = types.InlineQueryResultArticle('1', 'default', types.InputTextMessageContent("1"))
        c = types.InlineQueryResultArticle('2', 'test', types.InputTextMessageContent(lambda query: query.query))
        bot.answer_inline_query(inline_query.id, [r,c])
    except Exception as e:
        print(e)


def main_loop():
    bot.polling(True)
    while 1:
        time.sleep(3)


if __name__ == '__main__':
    try:
        main_loop()
    except KeyboardInterrupt:
        print('\nExiting by user request.\n')
        sys.exit(0)
