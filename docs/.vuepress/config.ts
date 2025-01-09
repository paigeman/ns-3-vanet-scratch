import { defineUserConfig } from "vuepress";

import theme from "./theme.js";

export default defineUserConfig({
  base: "/ns-3-vanet-scratch/",

  locales: {
    "/": {
      lang: "en-US",
      title: "Docs",
      description: "A docs demo for ns-3-vanet-scratch",
    },
    "/zh/": {
      lang: "zh-CN",
      title: "文档",
      description: "ns-3-vanet-scratch 的文档",
    },
  },

  theme,

  // Enable it with pwa
  // shouldPrefetch: false,
});
