export interface Config {
  $schema: string;
  name: string;
  version: string;
  description?: string;
  variables?: Record<string, string>;
  targets?: Record<string, ConfigTarget>;
  projectDependencies?: Record<string, ConfigProjectDependency>;
}

export interface ConfigTarget {
  default?: boolean;
  in?: string | string[];
  transitive?: string | string[];
  cmd?: string | string[];
  cmdMulti?: boolean;
  variables?: Record<string, string>;
}

export interface ConfigProjectDependency {
  source?: string;
  install?: string | string[];
}
