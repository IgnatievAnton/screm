import telebot
import re
from telebot import types
import logging


API_TOKEN = ''

bot = telebot.TeleBot(API_TOKEN)
telebot.logger.setLevel(logging.DEBUG)
telebot.logger.setLevel(logging.DEBUG)


@bot.inline_handler(lambda query: query.query)
def query_text(inline_query):
    try:
        r = types.InlineQueryResultArticle('1', 'add Memos', types.InputTextMessageContent(str(inline_query.query)+"\nВсем здоровья и хорошего настроения!😊"))
        bot.answer_inline_query(inline_query.id, [r])
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
