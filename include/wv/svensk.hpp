#pragma once

#define storlek_av sizeof
#define justera_som alignas
#define justering_av alignof
#define deklarerad_typ decltype

#define mnt asm // montering

#define kompis friend
#define allmän public
#define privat private
#define skyddad protected

#define inline inline
#define export export
#define extern extern
#define tydlig explicit
#define operatör operator
#define som_inte_krashar noexcept

#define kassa register
#define koncept concept
#define kräver requires
#define föränderlig mutable

#define konstant const
#define konstantuttryck constexpr
#define konstantuppstart constinit
#define konstantutvärdering consteval
#define konstant_omvandling const_cast

#define dynamisk_omvandling dynamic_cast
#define omtolknings_omvandling reinterpret_cast

#define statisk static
#define statisk_assert static_assert
#define statisk_omvandling static_cast

#define klass class
#define mall template
#define struktur struct
#define uppräkning enum
#define namnrum namespace

#define union union
#define typid typeid
#define använd using
#define typnamn typename
#define virtuell virtual
#define instabil volatile
#define typdefinera typedef

#define och and
#define inte not
#define eller or
#define bitoch bitand
#define biteller bitor
#define och_likamed and_eq
#define inte_likamed not_eq
#define eller_likamed or_eq
#define exklusiv_eller xor
#define exklusiv_eller_likamed xor_eq

#define som_kan_gå_minus signed
#define som_inte_kan_gå_minus unsigned

#define dennahär this

#define tom void
#define automatisk auto

#define karaktär char
#define karaktär8_t char8_t
#define karaktär16_t char16_t
#define karaktär32_t char32_t
#define bred_karaktär_t wchar_t

#define decimaltal float
#define stor_decimaltal double

#define siffra int
#define stor_siffra long
#define liten_siffra short
#define större_siffra long long

#define bool bool
#define sannt true
#define falskt false

#define ny new
#define ta_bort delete

#define om if
#define gör do
#define för for
#define försök try
#define kasta throw
#define fånga catch
#define växla switch
#define medans while
#define avbryt break
#define annnars else
#define gå_till goto
#define ifall_att case
#define vanliga default
#define återvänd return
#define fortsätt continue

#define adress_av(_var) &_var
#define adress_till(_var) _var*
#define adress_som_inte_leder_någonstans nullptr

#define är =
#define då {
#define slut }

#define som_inte_tar_några_parametrar ()
#define som_tar_in (
#define som_parametrar )
#define radslut ;
#define är_likamed ==
#define plus +
#define minus -
#define delat_med /
#define gånger *

#define skrivare printf

#define noll 0
#define ett  1
#define två  2
#define tre  3
#define fyra 4
#define fem  5
#define sex  6
#define sju  7
#define åtta 8
#define nio  9
#define tio  10
#define elva 11
#define tolv 12
