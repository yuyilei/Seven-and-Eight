# coding: utf-8
from flask import render_template, jsonify
from flask_mail import Message, Mail
from celery.utils.log import get_task_logger
import time
from celery import Celery
from celery.schedules import crontab
from flask import Flask

"""
要求：
理解每行代码的意思，实现定时发送邮件的功能!!
时间:
一周
"""

app = Flask(__name__)
app.config['MAIL_SERVER'] = 'smtp.qq.com'
app.config['MAIL_PORT'] = 25
app.config['MAIL_USE_TLS'] = True
app.config['MAIL_DEBUG'] = True
app.config['MAIL_DEFAULT_SENDER'] = app.config['MAIL_USERNAME'] = 'XXXX'   # 邮件的发送者
app.config['MAIL_PASSWORD'] = 'XXXX'                                       # 邮箱的授权码

#CELERY_BROKER_URL = 'redis://redis1:6385/0'
#CELERY_BACKEND_URL = 'redis://redis1:6385/1'
CELERY_BROKER_URL = 'redis://localhost:6383/0'
CELERY_BACKEND_URL = 'redis://localhost:6383/1'


celery = Celery('mails',broker=CELERY_BROKER_URL,backend=CELERY_BACKEND_URL)
mails = Mail(app)


@celery.task
def send_async_email(msg_dict):
    with app.app_context():
        """
        异步方法
        """
        msg = Message()
        msg.__dict__.update(msg_dict)
        mails.send(msg)


def msg_dict2(to, subject, **kwargs):
    """
    生成邮件
    """
    msg = Message(
        subject='木犀团队 ' + subject,
        sender=app.config['MAIL_DEFAULT_SENDER'],
        recipients=[to]
    )
    msg.body = '您好，我是一封邮件'
    msg.html = '<p>您好, 我是一封邮件</p>'
    return msg.__dict__


def send_mail2(to, subject, **kwargs):
    """
    发送邮件
    """
    send_async_email.delay(msg_dict2(to, subject, **kwargs))

@celery.on_after_configure.connect
def setup_periodic_tasks(sender, **kwargs):
	# Calls send_async_email  every 10 seconds.
	sender.add_periodic_task(10.0,send_async_email.s(msg_dict2('XXX@qq.com',' 一封邮件')))


if __name__ == '__main__' :
    app.run()
