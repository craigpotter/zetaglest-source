// ==============================================================
//	This file is part of Glest Shared Library (www.glest.org)
//
//	Copyright (C) 2001-2008 Martiño Figueroa
//
//	You can redistribute this code and/or modify it under 
//	the terms of the GNU General Public License as published 
//	by the Free Software Foundation; either version 2 of the 
//	License, or (at your option) any later version
// ==============================================================


#ifndef _SHARED_GRAPHICS_VEC_H_
#define _SHARED_GRAPHICS_VEC_H_

#include "math_wrapper.h"
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>
#include <limits>
#include "byte_order.h"
//#include <tr1/unordered_map>
//using namespace std::tr1;

#include "data_types.h"
#include "leak_dumper.h"

using namespace std;
using namespace Shared::Platform;

namespace Shared {
	namespace Graphics {

		template<typename T>
		inline T truncateDecimal(const T &value, int precision = 6) {
			T precNum = 0;
			if (precision == 0) {
				precNum = 1;
			} else if (precision == 1) {
				precNum = 10;
			} else if (precision == 2) {
				precNum = 100;
			} else if (precision == 3) {
				precNum = 1000;
			} else if (precision == 4) {
				precNum = 10000;
			} else if (precision == 5) {
				precNum = 100000;
			} else if (precision == 6) {
				precNum = 1000000;
			} else {
				precNum = std::pow((T) 10, (T) precision);
			}

			// See if we can avoid using an int64 for speed
		// To avoid stupid VC++ compiler error: illegal token on right side of '::'
		//#ifdef WIN32
		//#undef max
		//#endif
		//	static int MAX_INT_VALUE = numeric_limits<int>::max();
		//	if((T)value * (T)precNum <= MAX_INT_VALUE) {
		//		int resultInt = (int)((T)value * (T)precNum);
		//		T result = (T)resultInt / precNum;
		//		//printf("=======================\nvalue = %.10f\nresultInt: %d\nprecision: %d\nbecame: %.10f\n----------\n",value,resultInt,precision,result);
		//		return result;
		//	}

			// Must use an int64 since the result is large
			int64 resultInt = (int64) ((T) value * (T) precNum);
			T result = (T) ((long double) resultInt / precNum);
			return result;
		}

		inline std::vector<std::string> TokenizeString(const std::string str, const std::string delimiters) {
			std::vector<std::string> tokens;
			// Assume textLine contains the line of text to parse.
			std::string textLine = str;

			std::size_t pos = 0;
			while (true) {
				std::size_t nextPos = textLine.find(delimiters, pos);
				if (nextPos == textLine.npos) {
					tokens.push_back(textLine.substr(pos, textLine.length()));
					break;
				}
				tokens.push_back(std::string(textLine.substr(pos, nextPos - pos)));
				pos = nextPos + 1;
			}

			return tokens;
		}

		template<typename T>
		inline T strToType(const std::string &s) {
			char *endChar = NULL;

			setlocale(LC_NUMERIC, "C");
			T value = static_cast<T>(strtod(s.c_str(), &endChar));

			if (*endChar != '\0') {
				throw std::runtime_error("Error converting from string to type, found: [" + s + "]");
			}

			return value;
		}


		template<typename T> class Vec2;
		template<typename T> class Vec3;
		template<typename T> class Vec4;

		template<class T>
		void toEndianVecArray(T *vec, size_t size) {
			static bool bigEndianSystem = Shared::PlatformByteOrder::isBigEndian();
			if (bigEndianSystem == true) {
				for (size_t i = 0; i < size; ++i) {
					vec[i].toEndian();
				}
			}
		}
		template<class T>
		void fromEndianVecArray(T *vec, size_t size) {
			static bool bigEndianSystem = Shared::PlatformByteOrder::isBigEndian();
			if (bigEndianSystem == true) {
				for (size_t i = 0; i < size; ++i) {
					vec[i].fromEndian();
				}
			}
		}

		// =====================================================
		//	class Vec2
		// =====================================================

		template<typename T>
		class Vec2 {
		public:
			T x;
			T y;

		public:
			Vec2() {
				x = 0;
				y = 0;
			};

			explicit Vec2(T *p) {
				this->x = p[0];
				this->y = p[1];
			}

			explicit Vec2(T xy) {
				this->x = xy;
				this->y = xy;
			}

			template<typename S>
			explicit Vec2(const Vec2<S> &v) {
				this->x = v.x;
				this->y = v.y;
			}
			template<typename S>
			explicit Vec2(Vec2<S> &v) {
				this->x = v.x;
				this->y = v.y;
			}

			Vec2(T x, T y) {
				this->x = x;
				this->y = y;
			}

			//template<typename S>
			//size_t operator()(const Vec2<S> &v) const {
			//	return hash<T>()(v.x) ^ hash<T>()(v.y);
			//}
			//template<typename S>
			//bool operator()(const Vec2<S> &a, const Vec2<S> &b) const {
			//	return a == b;
			//}

			inline T *ptr() {
				return reinterpret_cast<T*>(this);
			}

			inline const T *ptr() const {
				return reinterpret_cast<const T*>(this);
			}

			inline Vec2<T> & operator=(const Vec2<T> &v) {
				this->x = v.x;
				this->y = v.y;
				return *this;
			}

			inline bool operator ==(const Vec2<T> &v) const {
				return x == v.x && y == v.y;
			}

			inline bool operator !=(const Vec2<T> &v) const {
				return x != v.x || y != v.y;
			}

			inline Vec2<T> operator +(const Vec2<T> &v) const {
				return Vec2(x + v.x, y + v.y);
			}

			inline Vec2<T> operator -(const Vec2<T> &v) const {
				return Vec2(x - v.x, y - v.y);
			}

			inline Vec2<T> operator -() const {
				return Vec2(-x, -y);
			}

			inline Vec2<T> operator *(const Vec2<T> &v) const {
				return Vec2(x*v.x, y*v.y);
			}

			inline Vec2<T> operator *(T s) const {
				return Vec2(x*s, y*s);
			}

			inline Vec2<T> operator /(const Vec2<T> &v) const {
				return Vec2(x / v.x, y / v.y);
			}

			inline Vec2<T> operator /(T s) const {
				return Vec2(x / s, y / s);
			}

			inline Vec2<T> operator +=(const Vec2<T> &v) {
				x += v.x;
				y += v.y;
				return *this;
			}

			inline Vec2<T> operator -=(const Vec2<T> &v) {
				x -= v.x;
				y -= v.y;
				return *this;
			}

			inline Vec2<T> lerp(T t, const Vec2<T> &v) const {
				return *this + (v - *this)*t;
			}

			inline T dot(const Vec2<T> &v) const {
				return x*v.x + y*v.y;
			}

			inline float dist(const Vec2<T> &v) const {
				float distance = Vec2<T>(v - *this).length();
				return distance;
			}

			// strict week ordering, so Vec2<T> can be used as key for set<> or map<>
			inline bool operator<(const Vec2<T> &v) const {
				return x < v.x || (x == v.x && y < v.y);
			}

			inline float length() const {
#ifdef USE_STREFLOP
				float len = static_cast<float>(streflop::sqrt(static_cast<streflop::Simple>(x*x + y*y)));
#else
				float len = static_cast<float>(std::sqrt(static_cast<float>(x*x + y*y)));
				len = truncateDecimal<float>(len, 6);
#endif
				return len;
			}

			inline void normalize() {
				T m = length();
				x /= m;
				y /= m;
			}

			inline Vec2<T> rotate(float rad) {
				float  c = std::cos(rad),
					s = std::sin(rad);

				return Vec2<T>(x*c - y*s, x*s + y*c);
			}

			inline Vec2<T> rotateAround(float rad, const Vec2<T>& pt) {
				return pt + (*this - pt).rotate(rad);
			}

			inline std::string getString() const {
				std::ostringstream streamOut;
				streamOut << "x [" << x;
				streamOut << "] y [" << y << "]";
				std::string result = streamOut.str();
				streamOut.str(std::string());
				return result;
			}
			// meetingPos="x [32] y [120]"
			static inline Vec2<T> strToVec2(std::string value) {
				Vec2<T> result;

				std::vector<std::string> tokens = TokenizeString(value, "[");
				//for(unsigned int i = 0; i < tokens.size(); ++i) {
					//printf("#1 Vec2T i = %d [%s]\n",i,tokens[i].c_str());
				//}
				if (tokens.size() == 3) {
					std::vector<std::string> tokens2 = TokenizeString(tokens[1], "]");
					//for(unsigned int i = 0; i < tokens2.size(); ++i) {
						//printf("#2 Vec2T i = %d [%s]\n",i,tokens2[i].c_str());
					//}
					std::vector<std::string> tokens3 = TokenizeString(tokens[2], "]");
					//for(unsigned int i = 0; i < tokens3.size(); ++i) {
						//printf("#3 Vec2T i = %d [%s]\n",i,tokens3[i].c_str());
					//}

					if (tokens2.size() == 2 && tokens3.size() == 2) {
						result.x = (T) strToType<T>(tokens2[0]);
						result.y = (T) strToType<T>(tokens3[0]);

						//printf("#3 Vec2T [%s]\n",result.getString().c_str());
					}
				}


				return result;
			}

			void toEndian() {
				static bool bigEndianSystem = Shared::PlatformByteOrder::isBigEndian();
				if (bigEndianSystem == true) {
					this->x = Shared::PlatformByteOrder::toCommonEndian(this->x);
					this->y = Shared::PlatformByteOrder::toCommonEndian(this->y);
				}
			}
			void fromEndian() {
				static bool bigEndianSystem = Shared::PlatformByteOrder::isBigEndian();
				if (bigEndianSystem == true) {
					this->x = Shared::PlatformByteOrder::fromCommonEndian(this->x);
					this->y = Shared::PlatformByteOrder::fromCommonEndian(this->y);
				}
			}

		};

		template <typename T>
		inline std::ostream& operator<<(std::ostream &stream, const Vec2<T> &vec) {
			return stream << "(" << vec.x << ", " << vec.y << ")";
		}


		typedef Vec2<int> Vec2i;
		typedef Vec2<bool> Vec2b;
		typedef Vec2<char> Vec2c;
		typedef Vec2<float> Vec2f;
		typedef Vec2<double> Vec2d;

		// =====================================================
		//	class Vec3
		// =====================================================

		template<typename T>
		class Vec3 {
		public:
			T x;
			T y;
			T z;

		public:
			Vec3() {
				x = 0;
				y = 0;
				z = 0;
			};

			explicit Vec3(T *p) {
				this->x = p[0];
				this->y = p[1];
				this->z = p[2];
			}

			explicit Vec3(T xyz) {
				this->x = xyz;
				this->y = xyz;
				this->z = xyz;
			}

			template<typename S>
			explicit Vec3(const Vec3<S> &v) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
			}

			template<typename S>
			explicit Vec3(Vec3<S> &v) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
			}

			Vec3(T x, T y, T z) {
				this->x = x;
				this->y = y;
				this->z = z;
			}

			explicit Vec3(Vec4<T> v) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
			}

			inline T *ptr() {
				return reinterpret_cast<T*>(this);
			}

			inline const T *ptr() const {
				return reinterpret_cast<const T*>(this);
			}

			inline Vec3<T> & operator=(const Vec3<T> &v) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
				return *this;
			}

			inline bool operator ==(const Vec3<T> &v) const {
				return x == v.x && y == v.y && z == v.z;
			}

			inline bool operator !=(const Vec3<T> &v) const {
				return x != v.x || y != v.y || z != v.z;
			}

			inline Vec3<T> operator +(const Vec3<T> &v) const {
				return Vec3(x + v.x, y + v.y, z + v.z);
			}

			inline Vec3<T> operator -(const Vec3<T> &v) const {
				return Vec3(x - v.x, y - v.y, z - v.z);
			}

			inline Vec3<T> operator -() const {
				return Vec3(-x, -y, -z);
			}

			inline Vec3<T> operator *(const Vec3<T> &v) const {
				return Vec3(x*v.x, y*v.y, z*v.z);
			}

			inline Vec3<T> operator *(T s) const {
				return Vec3(x*s, y*s, z*s);
			}

			inline Vec3<T> operator /(const Vec3<T> &v) const {
				return Vec3(x / v.x, y / v.y, z / v.z);
			}

			inline Vec3<T> operator /(T s) const {
				return Vec3(x / s, y / s, z / s);
			}

			inline Vec3<T> operator +=(const Vec3<T> &v) {
				x += v.x;
				y += v.y;
				z += v.z;
				return *this;
			}

			inline Vec3<T> operator -=(const Vec3<T> &v) {
				x -= v.x;
				y -= v.y;
				z -= v.z;
				return *this;
			}

			inline bool operator <(const Vec3<T> &v) const {
				return x < v.x || (x == v.x && y < v.y) || (x == v.x && y == v.y && z < v.z);
			}

			inline Vec3<T> lerp(T t, const Vec3<T> &v) const {
				return *this + (v - *this) * t;
			}

			inline T dot(const Vec3<T> &v) const {
				return x*v.x + y*v.y + z*v.z;
			}

			inline float dist(const Vec3<T> &v) const {
				float distance = Vec3<T>(v - *this).length();
				return distance;
			}

			inline float length() const {
#ifdef USE_STREFLOP
				float len = static_cast<float>(streflop::sqrt(static_cast<streflop::Simple>(x*x + y*y + z*z)));
#else
				float len = static_cast<float>(std::sqrt(x*x + y*y + z*z));
				len = truncateDecimal<float>(len, 6);
#endif
				return len;
			}

			inline void normalize() {
				T m = length();
				x /= m;
				y /= m;
				z /= m;
			}

			inline Vec3<T> getNormalized() const {
				T m = length();
				return Vec3<T>(x / m, y / m, z / m);
			}

			inline Vec3<T> cross(const Vec3<T> &v) const {
				return Vec3<T>(
					this->y*v.z - this->z*v.y,
					this->z*v.x - this->x*v.z,
					this->x*v.y - this->y*v.x);
			}

			inline Vec3<T> normal(const Vec3<T> &p1, const Vec3<T> &p2) const {
				Vec3<T> rv;
				rv = (p2 - *this).cross(p1 - *this);
				rv.normalize();
				return rv;
			}

			inline Vec3<T> normal(const Vec3<T> &p1, const Vec3<T> &p2, const Vec3<T> &p3, const Vec3<T> &p4) const {
				Vec3<T> rv;

				rv = this->normal(p1, p2);
				rv = rv + this->normal(p2, p3);
				rv = rv + this->normal(p3, p4);
				rv = rv + this->normal(p4, p1);
				rv.normalize();
				return rv;
			}

			inline std::string getString() const {
				std::ostringstream streamOut;
				streamOut << "x [" << x;
				streamOut << "] y [" << y;
				streamOut << "] z [" << z << "]";
				std::string result = streamOut.str();
				streamOut.str(std::string());
				return result;
			}

			// playerColor="x [1] y [0] z [0]"
			static inline Vec3<T> strToVec3(std::string value) {
				Vec3<T> result;

				std::vector<std::string> tokens = TokenizeString(value, "[");
				//for(unsigned int i = 0; i < tokens.size(); ++i) {
					//printf("#1 Vec2T i = %d [%s]\n",i,tokens[i].c_str());
				//}
				if (tokens.size() == 4) {
					std::vector<std::string> tokens2 = TokenizeString(tokens[1], "]");
					//for(unsigned int i = 0; i < tokens2.size(); ++i) {
						//printf("#2 Vec2T i = %d [%s]\n",i,tokens2[i].c_str());
					//}
					std::vector<std::string> tokens3 = TokenizeString(tokens[2], "]");
					//for(unsigned int i = 0; i < tokens3.size(); ++i) {
						//printf("#3 Vec2T i = %d [%s]\n",i,tokens3[i].c_str());
					//}

					std::vector<std::string> tokens4 = TokenizeString(tokens[3], "]");
					//for(unsigned int i = 0; i < tokens3.size(); ++i) {
						//printf("#3 Vec2T i = %d [%s]\n",i,tokens3[i].c_str());
					//}

					if (tokens2.size() == 2 && tokens3.size() == 2 && tokens4.size() == 2) {
						result.x = (T) strToType<T>(tokens2[0]);
						result.y = (T) strToType<T>(tokens3[0]);
						result.z = (T) strToType<T>(tokens4[0]);

						//printf("#3 Vec2T [%s]\n",result.getString().c_str());
					}
				}


				return result;
			}

			void toEndian() {
				static bool bigEndianSystem = Shared::PlatformByteOrder::isBigEndian();
				if (bigEndianSystem == true) {
					this->x = Shared::PlatformByteOrder::toCommonEndian(this->x);
					this->y = Shared::PlatformByteOrder::toCommonEndian(this->y);
					this->z = Shared::PlatformByteOrder::toCommonEndian(this->z);
				}
			}
			void fromEndian() {
				static bool bigEndianSystem = Shared::PlatformByteOrder::isBigEndian();
				if (bigEndianSystem == true) {
					this->x = Shared::PlatformByteOrder::fromCommonEndian(this->x);
					this->y = Shared::PlatformByteOrder::fromCommonEndian(this->y);
					this->z = Shared::PlatformByteOrder::fromCommonEndian(this->z);
				}
			}

		};

		typedef Vec3<int> Vec3i;
		typedef Vec3<bool> Vec3b;
		typedef Vec3<char> Vec3c;
		typedef Vec3<float> Vec3f;
		typedef Vec3<double> Vec3d;

		// =====================================================
		//	class Vec4
		// =====================================================

		template<typename T>
		class Vec4 {
		public:
			T x;
			T y;
			T z;
			T w;
		public:
			Vec4() {
				x = 0;
				y = 0;
				z = 0;
				w = 0;
			};

			explicit Vec4(const T *p) {
				this->x = p[0];
				this->y = p[1];
				this->z = p[2];
				this->w = p[3];
			}

			explicit Vec4(T *p) {
				this->x = p[0];
				this->y = p[1];
				this->z = p[2];
				this->w = p[3];
			}

			explicit Vec4(T xyzw) {
				this->x = xyzw;
				this->y = xyzw;
				this->z = xyzw;
				this->w = xyzw;
			}

			template<typename S>
			explicit Vec4(const Vec4<S> &v) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
				this->w = v.w;
			}

			template<typename S>
			explicit Vec4(Vec4<S> &v) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
				this->w = v.w;
			}

			Vec4(T x, T y, T z, T w) {
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}

			Vec4(Vec3<T> v, T w) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
				this->w = w;
			}

			explicit Vec4(Vec3<T> v) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
				this->w = 1;
			}

			inline T *ptr() {
				return reinterpret_cast<T*>(this);
			}

			inline const T *ptr() const {
				return reinterpret_cast<const T*>(this);
			}

			inline Vec4<T> & operator=(const Vec4<T> &v) {
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
				this->w = v.w;
				return *this;
			}

			inline bool operator ==(const Vec4<T> &v) const {
				return x == v.x && y == v.y && z == v.z && w == v.w;
			}

			inline bool operator !=(const Vec4<T> &v) const {
				return x != v.x || y != v.y || z != v.z || w != v.w;
			}

			inline Vec4<T> operator +(const Vec4<T> &v) const {
				return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
			}

			inline Vec4<T> operator -(const Vec4<T> &v) const {
				return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
			}

			inline Vec4<T> operator -() const {
				return Vec4(-x, -y, -z, -w);
			}

			inline Vec4<T> operator *(const Vec4<T> &v) const {
				return Vec4(x*v.x, y*v.y, z*v.z, w*v.w);
			}

			inline Vec4<T> operator *(T s) const {
				return Vec4(x*s, y*s, z*s, w*s);
			}

			inline Vec4<T> operator /(const Vec4<T> &v) const {
				return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);
			}

			inline Vec4<T> operator /(T s) const {
				return Vec4(x / s, y / s, z / s, w / s);
			}

			inline Vec4<T> operator +=(const Vec4<T> &v) {
				x += v.x;
				y += v.y;
				z += v.z;
				w += v.w;
				return *this;
			}

			inline Vec4<T> operator -=(const Vec4<T> &v) {
				x -= v.x;
				y -= v.y;
				z -= v.z;
				w -= v.w;
				return *this;
			}
			inline bool operator <(const Vec4<T> &v) const {
				return x < v.x || (x == v.x && y < v.y) ||
					(x == v.x && y == v.y && z < v.z) ||
					(x == v.x && y == v.y && z == v.z && w < v.w);
			}

			inline Vec4<T> lerp(T t, const Vec4<T> &v) const {
				return *this + (v - *this) *t;
			}

			inline T dot(const Vec4<T> &v) const {
				return x*v.x + y*v.y + z*v.z + w*v.w;
			}

			inline std::string getString() const {
				std::ostringstream streamOut;
				streamOut << "x [" << x;
				streamOut << "] y [" << y;
				streamOut << "] z [" << z;
				streamOut << "] w [" << w << "]";
				std::string result = streamOut.str();
				streamOut.str(std::string());
				return result;
			}

			// playerColor="x [1] y [0] z [0] w [0]"
			static inline Vec4<T> strToVec4(std::string value) {
				Vec4<T> result;

				std::vector<std::string> tokens = TokenizeString(value, "[");
				//for(unsigned int i = 0; i < tokens.size(); ++i) {
					//printf("#1 Vec2T i = %d [%s]\n",i,tokens[i].c_str());
				//}
				if (tokens.size() == 5) {
					std::vector<std::string> tokens2 = TokenizeString(tokens[1], "]");
					//for(unsigned int i = 0; i < tokens2.size(); ++i) {
						//printf("#2 Vec2T i = %d [%s]\n",i,tokens2[i].c_str());
					//}
					std::vector<std::string> tokens3 = TokenizeString(tokens[2], "]");
					//for(unsigned int i = 0; i < tokens3.size(); ++i) {
						//printf("#3 Vec2T i = %d [%s]\n",i,tokens3[i].c_str());
					//}

					std::vector<std::string> tokens4 = TokenizeString(tokens[3], "]");
					//for(unsigned int i = 0; i < tokens3.size(); ++i) {
						//printf("#3 Vec2T i = %d [%s]\n",i,tokens3[i].c_str());
					//}

					std::vector<std::string> tokens5 = TokenizeString(tokens[4], "]");
					//for(unsigned int i = 0; i < tokens3.size(); ++i) {
						//printf("#3 Vec2T i = %d [%s]\n",i,tokens3[i].c_str());
					//}

					if (tokens2.size() == 2 && tokens3.size() == 2 &&
						tokens4.size() == 2 && tokens5.size() == 2) {
						result.x = (T) strToType<T>(tokens2[0]);
						result.y = (T) strToType<T>(tokens3[0]);
						result.z = (T) strToType<T>(tokens4[0]);
						result.w = (T) strToType<T>(tokens5[0]);

						//printf("#3 Vec2T [%s]\n",result.getString().c_str());
					}
				}

				return result;
			}

			void toEndian() {
				static bool bigEndianSystem = Shared::PlatformByteOrder::isBigEndian();
				if (bigEndianSystem == true) {
					this->x = Shared::PlatformByteOrder::toCommonEndian(this->x);
					this->y = Shared::PlatformByteOrder::toCommonEndian(this->y);
					this->z = Shared::PlatformByteOrder::toCommonEndian(this->z);
					this->w = Shared::PlatformByteOrder::toCommonEndian(this->w);
				}
			}
			void fromEndian() {
				static bool bigEndianSystem = Shared::PlatformByteOrder::isBigEndian();
				if (bigEndianSystem == true) {
					this->x = Shared::PlatformByteOrder::fromCommonEndian(this->x);
					this->y = Shared::PlatformByteOrder::fromCommonEndian(this->y);
					this->z = Shared::PlatformByteOrder::fromCommonEndian(this->z);
					this->w = Shared::PlatformByteOrder::fromCommonEndian(this->w);
				}
			}

		};

		typedef Vec4<int> Vec4i;
		typedef Vec4<bool> Vec4b;
		typedef Vec4<char> Vec4c;
		typedef Vec4<float> Vec4f;
		typedef Vec4<double> Vec4d;

	}
} //enmd namespace

#endif
