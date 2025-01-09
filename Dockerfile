FROM node:22.12.0 AS build
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY ./docs ./docs
COPY tsconfig.json ./
RUN npm run docs:build
FROM nginx:1.27.3
COPY nginx.conf /etc/nginx/nginx.conf
COPY --from=build /app/docs/.vuepress/dist /usr/share/nginx/html
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
