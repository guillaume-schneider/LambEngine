import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

const sidebars: SidebarsConfig = {
  docs: [
    {type: 'doc', id: 'index'},
    {type: 'doc', id: 'architecture'},
    {
      type: 'category',
      label: 'Getting Started',
      items: [
        'getting-started/installation',
        'getting-started/quick-start',
        'getting-started/project-structure',
      ],
    },
    {
      type: 'category',
      label: 'Guides',
      items: [
        'guides/rendering',
        'guides/shaders',
        'guides/input-system',
        'guides/ecs',
        'guides/configuration',
      ],
    },
    {
      type: 'category',
      label: 'API Reference',
      items: [
        'api/core',
        'api/renderer',
        'api/input',
        'api/utils',
      ],
    },
  ],
};

export default sidebars;
