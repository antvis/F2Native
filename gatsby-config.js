const { repository } = require('./package.json');

module.exports = {
  plugins: [
    {
      resolve: '@antv/gatsby-theme-antv',
      options: {
        // eslint-disable-next-line quotes
        GATrackingId: `UA-148148901-2`
      }
    }
  ],
  siteMetadata: {
    title: 'F2Native',
    description: 'The Grammar of Graphics in c++',
    siteUrl: 'https://f2native.antv.vision',
    githubUrl: repository.url,
    navs: [
      {
        slug: 'docs/tutorial',
        title: {
          zh: '使用文档',
          en: 'Manual'
        }
      },
      {
        slug: 'docs/api',
        title: {
          zh: 'API 文档',
          en: 'API'
        }
      },
      {
        slug: 'docs/examples/',
        title: {
          zh: '图表示例',
          en: 'Examples'
        }
      }
    ],
    docs: [
      {
        slug: 'tutorial/manual',
        title: {
          zh: '教程',
          en: 'Tutorial'
        },
        order: 1
      },
      {
        slug: 'api/chart',
        title: {
          zh: 'F2Chart',
          en: 'F2Chart'
        },
        order: 2
      },
      {
        slug: 'examples/line',
        icon: 'line',
        title: {
          zh: '折线图',
          en: 'Line Charts'
        },
        order: 0
      },
      {
        slug: 'examples/area',
        icon: 'area',
        title: {
          zh: '面积图',
          en: 'Area Charts'
        },
        order: 2
      },
      {
        slug: 'examples/column',
        icon: 'column',
        title: {
          zh: '柱状图',
          en: 'Column Charts'
        },
        order: 3
      }
    ],
    showSearch: false,
    playground: {
      container: '<div id="container" />',
      playgroundDidMount: 'console.log("playgroundDidMount");',
      playgroundWillUnmount: 'console.log("playgroundWillUnmount");'
    },
    docsearchOptions: {
      apiKey: '349619374e9488b3f000714317d67381',
      indexName: 'antv_f2native'
    },
    redirects: []
  }
};
