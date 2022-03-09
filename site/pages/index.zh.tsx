import React from 'react';
import SEO from '@antv/gatsby-theme-antv/site/components/Seo';
import { useTranslation } from 'react-i18next';
import Banner from '@antv/gatsby-theme-antv/site/components/Banner';
import Companies from '@antv/gatsby-theme-antv/site/components/Companies';
import Features from '@antv/gatsby-theme-antv/site/components/Features';
import Cases from '@antv/gatsby-theme-antv/site/components/Cases';
import BannerSVG from '@antv/gatsby-theme-antv/site/components/BannerSVG';

const IndexPage = () => {
  const { t, i18n } = useTranslation();

  const features = [
    {
      icon:
        'https://gw.alipayobjects.com/zos/basement_prod/5dbaf094-c064-4a0d-9968-76020b9f1510.svg',
      title: t('专注客户端，极致体验'),
      description: t(
        '围绕设计、性能以及不同平台环境，为用户提供移动客户端图表的最佳实践',
      ),
    },
    {
      icon:
        'https://gw.alipayobjects.com/zos/basement_prod/0a0371ab-6bed-41ad-a99b-87a5044ba11b.svg',
      title: t('图表丰富，组件完备'),
      description: t(
        '基于图形语法，灵活构建各类图表；组件完备，覆盖各类场景',
      ),
    },
    {
      icon:
        'https://gw.alipayobjects.com/zos/basement_prod/716d0bc0-e311-4b28-b79f-afdd16e8148e.svg',
      title: t('轻量化植入，自然呈现'),
      description: t(
        '设计上自然简单易懂,可视化展现和人的自然行为特征有机融合;引入灵活、学习成本低',
      ),
    },
  ];

  const companies = [
    { name: '蚂蚁财富', img: 'https://gw.alipayobjects.com/zos/finxbff/compress-tinypng/89790c1c-cc08-4270-8489-99893a57b20c.png' },
    { name: '支付宝', img: 'https://gw.alipayobjects.com/zos/finxbff/compress-tinypng/0014f3ad-ec8c-4023-97b4-740090cd78a2.png', },
  ];

  const cases = [
    {
      logo: 'https://antv.alipay.com/assets/image/home/f2/caifu-logo.png',
      title: t('蚂蚁财富'),
      description: t('已广泛应用于股票等金融业务场景中，支撑着蚂蚁财富 app 上众多可视化场景。同时通过深入剖析用户的可视化诉求，沉淀出多套面向金融的可视化方案。'),
      image: 'https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*W8dZTIH9hUAAAAAAAAAAAAAAARQnAQ',
    },
    {
      logo: 'https://antv.alipay.com/assets/image/home/f2/alipay-logo.png',
      title: t('支付宝'),
      description: t('覆盖证券、新闻财报等多个场景,在效能提升和高性能体验等方面都有着优秀的表现'),
      image: 'https://gw.alipayobjects.com/mdn/rms_04a9e5/afts/img/A*x8WhQZAUfjgAAAAAAAAAAAAAARQnAQ',
    },
  ];

  const bannerButtons = [
    {
      text: t('开始使用'),
      link: `/${i18n.language}/docs/tutorial/getting-started`,
      type: 'primary',
    },
    {
      text: t('图表演示'),
      link: `/${i18n.language}/docs/examples/`,
    },
  ];

  return (
    <>
      <SEO title={t('F2Native 移动端可视化方案')} titleSuffix="AntV" lang={i18n.language} />
      <Banner
        coverImage={
          <img
            width="100%"
            className="Notification-module--number--31-3Z"
            style={{ marginTop: '70px' }}
            src="https://gw.alipayobjects.com/zos/finxbff/compress-tinypng/870e2e92-6187-4729-889c-232a7df0998a.png"
          />
        }
        title={t('F2Native 移动端可视化方案')}
        description={t(
          'F2Native 是一个专注于客户端，开箱即用、高性能的可视化解决方案，支持 Android，iOS和MacOS 平台，完备的图形语法理论，满足你的各种需求，专业的移动设计指引为你带来最佳的移动端图表体验。',
        )}
        className="banner"
        buttons={bannerButtons}
      />
      <Features
        features={features}
        style={{ width: '100%' }}
      />
      <Cases cases={cases} />
      <Companies
        title={t('感谢信赖')}
        companies={companies}
      />
    </>
  );
};

export default IndexPage;
