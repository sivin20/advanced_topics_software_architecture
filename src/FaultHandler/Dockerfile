FROM gradle:8.1.0-jdk17-alpine AS build
LABEL authors="hannehue"
COPY --chown=gradle:gradle . /home/gradle/src
WORKDIR /home/gradle/src
RUN gradle bootJar

FROM eclipse-temurin:17-jdk-alpine
LABEL authors="hannehue"
VOLUME /tmp
COPY --from=build /home/gradle/src/build/libs/*.jar faultHandler.jar
ENTRYPOINT ["java", "-jar", "/faultHandler.jar", "--error"]