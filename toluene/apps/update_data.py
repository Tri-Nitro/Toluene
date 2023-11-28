#!/usr/bin/env python

import argparse
import requests
import yaml


def main():
    parser = create_arg_parser()
    args = parser.parse_args()
    config_file = args.config_file

    config = None
    with open(config_file, 'r') as file:
        config = yaml.safe_load(file)

    save_data(config, args.output_dir)


def create_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description='Grabs the latest data from the online sources')
    parser.add_argument('config_file', type=str, help='The config file with the online sources')
    parser.add_argument('--output_dir', type=str, help='The directory to output the config files to.',
                        default='.')
    return parser


def save_data(config: yaml, output_dir: str):

    with open("{}/deltat.data".format(output_dir), 'w') as deltat_data:
        response = requests.get(config['delta_t']['url'])
        deltat_data.write(response.text)

    with open("{}/finals2000A.all".format(output_dir), 'w') as finals2000A_all:
        response = requests.get(config['earth_orientation']['url'])
        finals2000A_all.write(response.text)


if __name__ == '__main__':
    main()
