# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                                   #
#   MIT License                                                                     #
#                                                                                   #
#   Copyright (c) 2023 Tri-Nitro                                                    #
#                                                                                   #
#   Permission is hereby granted, free of charge, to any person obtaining a copy    #
#   of this software and associated documentation files (the "Software"), to deal   #
#   in the Software without restriction, including without limitation the rights    #
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       #
#   copies of the Software, and to permit persons to whom the Software is           #
#   furnished to do so, subject to the following conditions:                        #
#                                                                                   #
#   The above copyright notice and this permission notice shall be included in all  #
#   copies or substantial portions of the Software.                                 #
#                                                                                   #
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      #
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        #
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     #
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          #
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   #
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   #
#   SOFTWARE.                                                                       #
#                                                                                   #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
from typing import List

from toluene_extensions.models.earth.coefficients import new_CIRStoTIRSCoefficients, load_zeta_a, load_z_a, load_theta_a, \
    load_psi_a, load_omega_a, load_epsilon_a, load_chi_a, load_l, load_l_prime, load_F, load_D, load_Omega, load_l_me, \
    load_l_v, load_l_e, load_l_ma, load_l_j, load_l_s, load_l_u, load_l_n, load_p


class CIRStoTIRSCoefficients:

    def __init__(self, cirs_to_tirs_dict: dict = None):
        self.__coefficients = None
        if cirs_to_tirs_dict is not None:
            self.load_from_dict(cirs_to_tirs_dict)

    def load_from_dict(self, cirs_to_tirs_dict: dict):
        self.__coefficients = new_CIRStoTIRSCoefficients()

        if 'zeta_a' in cirs_to_tirs_dict:
            self.__send_zeta_a_to_c(cirs_to_tirs_dict['zeta_a'])

        if 'z_a' in cirs_to_tirs_dict:
            self.__send_z_a_to_c(cirs_to_tirs_dict['z_a'])

        if 'theta_a' in cirs_to_tirs_dict:
            self.__send_theta_a_to_c(cirs_to_tirs_dict['theta_a'])

        if 'psi_a' in cirs_to_tirs_dict:
            self.__send_psi_a_to_c(cirs_to_tirs_dict['psi_a'])

        if 'omega_a' in cirs_to_tirs_dict:
            self.__send_omega_a_to_c(cirs_to_tirs_dict['omega_a'])

        if 'epsilon_a' in cirs_to_tirs_dict:
            self.__send_epsilon_a_to_c(cirs_to_tirs_dict['epsilon_a'])

        if 'chi_a' in cirs_to_tirs_dict:
            self.__send_chi_a_to_c(cirs_to_tirs_dict['chi_a'])

        if 'l' in cirs_to_tirs_dict:
            self.__send_l_to_c(cirs_to_tirs_dict['l'])

        if 'l_prime' in cirs_to_tirs_dict:
            self.__send_l_prime_to_c(cirs_to_tirs_dict['l_prime'])

        if 'F' in cirs_to_tirs_dict:
            self.__send_F_to_c(cirs_to_tirs_dict['F'])

        if 'D' in cirs_to_tirs_dict:
            self.__send_D_to_c(cirs_to_tirs_dict['D'])

        if 'Omega' in cirs_to_tirs_dict:
            self.__send_Omega_to_c(cirs_to_tirs_dict['Omega'])

        if 'l_me' in cirs_to_tirs_dict:
            self.__send_l_me_to_c(cirs_to_tirs_dict['l_me'])

        if 'l_v' in cirs_to_tirs_dict:
            self.__send_l_v_to_c(cirs_to_tirs_dict['l_v'])

        if 'l_e' in cirs_to_tirs_dict:
            self.__send_l_e_to_c(cirs_to_tirs_dict['l_e'])

        if 'l_ma' in cirs_to_tirs_dict:
            self.__send_l_ma_to_c(cirs_to_tirs_dict['l_ma'])

        if 'l_j' in cirs_to_tirs_dict:
            self.__send_l_j_to_c(cirs_to_tirs_dict['l_j'])

        if 'l_s' in cirs_to_tirs_dict:
            self.__send_l_s_to_c(cirs_to_tirs_dict['l_s'])

        if 'l_u' in cirs_to_tirs_dict:
            self.__send_l_u_to_c(cirs_to_tirs_dict['l_u'])

        if 'l_n' in cirs_to_tirs_dict:
            self.__send_l_n_to_c(cirs_to_tirs_dict['l_n'])

        if 'p' in cirs_to_tirs_dict:
            self.__send_p_to_c(cirs_to_tirs_dict['p'])

    @property
    def coefficients(self):
        return self.__coefficients

    def __send_zeta_a_to_c(self, zeta_a: List[float]):
        load_zeta_a(self.__coefficients, zeta_a)

    def __send_z_a_to_c(self, z_a: List[float]):
        load_z_a(self.__coefficients, z_a)

    def __send_theta_a_to_c(self, theta_a: List[float]):
        load_theta_a(self.__coefficients, theta_a)

    def __send_psi_a_to_c(self, psi_a: List[float]):
        load_psi_a(self.__coefficients, psi_a)

    def __send_omega_a_to_c(self, omega_a: List[float]):
        load_omega_a(self.__coefficients, omega_a)

    def __send_epsilon_a_to_c(self, epsilon_a: List[float]):
        load_epsilon_a(self.__coefficients, epsilon_a)

    def __send_chi_a_to_c(self, chi_a: List[float]):
        load_chi_a(self.__coefficients, chi_a)

    def __send_l_to_c(self, l: List[float]):
        load_l(self.__coefficients, l)

    def __send_l_prime_to_c(self, l_prime: List[float]):
        load_l_prime(self.__coefficients, l_prime)

    def __send_F_to_c(self, F: List[float]):
        load_F(self.__coefficients, F)

    def __send_D_to_c(self, D: List[float]):
        load_D(self.__coefficients, D)

    def __send_Omega_to_c(self, Omega: List[float]):
        load_Omega(self.__coefficients, Omega)

    def __send_l_me_to_c(self, l_me: List[float]):
        load_l_me(self.__coefficients, l_me)

    def __send_l_v_to_c(self, l_v: List[float]):
        load_l_v(self.__coefficients, l_v)

    def __send_l_e_to_c(self, l_e: List[float]):
        load_l_e(self.__coefficients, l_e)

    def __send_l_ma_to_c(self, l_ma: List[float]):
        load_l_ma(self.__coefficients, l_ma)

    def __send_l_j_to_c(self, l_j: List[float]):
        load_l_j(self.__coefficients, l_j)

    def __send_l_s_to_c(self, l_s: List[float]):
        load_l_s(self.__coefficients, l_s)

    def __send_l_u_to_c(self, l_u: List[float]):
        load_l_u(self.__coefficients, l_u)

    def __send_l_n_to_c(self, l_n: List[float]):
        load_l_n(self.__coefficients, l_n)

    def __send_p_to_c(self, p: List[float]):
        load_p(self.__coefficients, p)
