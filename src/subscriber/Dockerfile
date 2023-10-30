FROM python:3.7-alpine

WORKDIR /usr/app/src

COPY src/subscriber.py ./
RUN pip install paho-mqtt
RUN pip install mysql-connector-python

CMD ["python", "./subscriber.py"]
