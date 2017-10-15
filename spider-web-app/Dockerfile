# Use ubuntu image
FROM ubuntu

# Authors
LABEL maintainer="thomas.lambert@epitech.eu"
LABEL maintainer="antoine.bache@epitech.eu"
LABEL maintainer="lucas.troncy@epitech.eu"
LABEL maintainer="marwane.khsime@epitech.eu"

# Install Depedencies
RUN apt-get update
RUN apt-get install -y build-essential apt-utils
RUN apt-get install -y  wget    \
                        git     \
                        gcc     \
                        clang   \
                        g++
RUN npm install

# Create the project's folder in the container
RUN mkdir -p /home/Project
# Copy the project in the container
COPY . /home/Project

EXPOSE 8080
CMD [ "npm", "start" ]