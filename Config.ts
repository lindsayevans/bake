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
  in?: string[];
  transitive?: string[];
  cmd?: string[];
  variables?: Record<string, string>;
}

export interface ConfigProjectDependency {
  source?: string;
  install?: string[];
}
